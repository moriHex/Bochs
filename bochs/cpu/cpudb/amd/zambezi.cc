/////////////////////////////////////////////////////////////////////////
// $Id$
/////////////////////////////////////////////////////////////////////////
//
//   Copyright (c) 2012-2017 Stanislav Shwartsman
//          Written by Stanislav Shwartsman [sshwarts at sourceforge net]
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA B 02110-1301 USA
//
/////////////////////////////////////////////////////////////////////////

#include "bochs.h"
#include "cpu.h"
#include "gui/siminterface.h"
#include "param_names.h"
#include "zambezi.h"

#define LOG_THIS cpu->

#if BX_SUPPORT_X86_64 && BX_SUPPORT_AVX

zambezi_t::zambezi_t(BX_CPU_C *cpu): bx_cpuid_t(cpu)
{
  BX_INFO(("WARNING: Light Weight Profiling (LWP) support is not implemented !"));

  if (! BX_SUPPORT_SVM)
    BX_INFO(("WARNING: SVM support is not compiled in !"));

  if (! BX_SUPPORT_MONITOR_MWAIT)
    BX_INFO(("WARNING: MONITOR/MWAIT support is not compiled in !"));

  enable_cpu_extension(BX_ISA_X87);
  enable_cpu_extension(BX_ISA_486);
  enable_cpu_extension(BX_ISA_PENTIUM);
  enable_cpu_extension(BX_ISA_P6);
  enable_cpu_extension(BX_ISA_MMX);
  enable_cpu_extension(BX_ISA_SYSCALL_SYSRET_LEGACY);
  enable_cpu_extension(BX_ISA_SYSENTER_SYSEXIT);
  enable_cpu_extension(BX_ISA_CLFLUSH);
  enable_cpu_extension(BX_ISA_DEBUG_EXTENSIONS);
  enable_cpu_extension(BX_ISA_VME);
  enable_cpu_extension(BX_ISA_PSE);
  enable_cpu_extension(BX_ISA_PAE);
  enable_cpu_extension(BX_ISA_PGE);
  enable_cpu_extension(BX_ISA_MTRR);
  enable_cpu_extension(BX_ISA_PAT);
  enable_cpu_extension(BX_ISA_XAPIC);
  enable_cpu_extension(BX_ISA_LONG_MODE);
  enable_cpu_extension(BX_ISA_LM_LAHF_SAHF);
  enable_cpu_extension(BX_ISA_CMPXCHG16B);
  enable_cpu_extension(BX_ISA_NX);
  enable_cpu_extension(BX_ISA_SSE);
  enable_cpu_extension(BX_ISA_SSE2);
  enable_cpu_extension(BX_ISA_SSE3);
  enable_cpu_extension(BX_ISA_SSSE3);
  enable_cpu_extension(BX_ISA_SSE4_1);
  enable_cpu_extension(BX_ISA_SSE4_2);
  enable_cpu_extension(BX_ISA_LZCNT);
  enable_cpu_extension(BX_ISA_POPCNT);
  enable_cpu_extension(BX_ISA_SSE4A);
#if BX_SUPPORT_MONITOR_MWAIT
  enable_cpu_extension(BX_ISA_MONITOR_MWAIT);
#endif
  enable_cpu_extension(BX_ISA_RDTSCP);
  enable_cpu_extension(BX_ISA_XSAVE);
  enable_cpu_extension(BX_ISA_AES_PCLMULQDQ);
  enable_cpu_extension(BX_ISA_AVX);
  enable_cpu_extension(BX_ISA_FMA4);
  enable_cpu_extension(BX_ISA_XOP);
#if BX_SUPPORT_SVM
  enable_cpu_extension(BX_ISA_SVM);
#endif
  enable_cpu_extension(BX_ISA_FFXSR);
  enable_cpu_extension(BX_ISA_1G_PAGES);
  enable_cpu_extension(BX_ISA_MISALIGNED_SSE);
  enable_cpu_extension(BX_ISA_ALT_MOV_CR8);
  enable_cpu_extension(BX_ISA_XAPIC_EXT);
}

void zambezi_t::get_cpuid_leaf(Bit32u function, Bit32u subfunction, cpuid_function_t *leaf) const
{
  static const char* brand_string = "AMD FX(tm)-4100 Quad-Core Processor            ";

  static bool cpuid_limit_winnt = SIM->get_param_bool(BXPN_CPUID_LIMIT_WINNT)->get();
  if (cpuid_limit_winnt)
    if (function > 1 && function < 0x80000000) function = 1;

  switch(function) {
  case 0x80000000:
    get_ext_cpuid_leaf_0(leaf);
    return;
  case 0x80000001:
    get_ext_cpuid_leaf_1(leaf);
    return;
  case 0x80000002:
  case 0x80000003:
  case 0x80000004:
    get_ext_cpuid_brand_string_leaf(brand_string, function, leaf);
    return;
  case 0x80000005:
    get_ext_cpuid_leaf_5(leaf);
    return;
  case 0x80000006:
    get_ext_cpuid_leaf_6(leaf);
    return;
  case 0x80000007:
    get_ext_cpuid_leaf_7(leaf);
    return;
  case 0x80000008:
    get_ext_cpuid_leaf_8(leaf);
    return;
#if BX_SUPPORT_SVM
  case 0x8000000A:
    get_ext_cpuid_leaf_A(leaf);
    return;
#endif
  case 0x80000019:
    get_ext_cpuid_leaf_19(leaf);
    return;
  case 0x8000001A: // CPUID leaf 0x8000001A - Performance Optimization Identifiers
    get_leaf(leaf, 0x00000003, 0x00000000, 0x00000000, 0x00000000);
    return;
  case 0x8000001B:
    get_ext_cpuid_leaf_1B(leaf);
    return;
  case 0x8000001C:
    get_ext_cpuid_leaf_1C(leaf);
    return;
  case 0x8000001D:
    get_ext_cpuid_leaf_1D(subfunction, leaf);
    return;
  case 0x8000001E:
    get_ext_cpuid_leaf_1E(leaf);
    return;
  case 0x00000000:
    get_std_cpuid_leaf_0(leaf);
    return;
  case 0x00000001:
    get_std_cpuid_leaf_1(leaf);
    return;
  case 0x00000005:
    get_std_cpuid_monitor_mwait_leaf(leaf, 0);
    return;
  case 0x00000006: // CPUID leaf 0x00000006 - Thermal and Power Management Leaf
    get_leaf(leaf, 0x00000000, 0x00000000, 0x00000001, 0x00000000);
    return;
  case 0x0000000D:
    get_std_cpuid_xsave_leaf(subfunction, leaf);
    return;
  default:
    get_reserved_leaf(leaf);
    return;
  }
}

#if BX_SUPPORT_SVM
Bit32u zambezi_t::get_svm_extensions_bitmask(void) const
{
  return BX_CPUID_SVM_NESTED_PAGING |
         BX_CPUID_SVM_LBR_VIRTUALIZATION |
         BX_CPUID_SVM_SVM_LOCK |
         BX_CPUID_SVM_NRIP_SAVE |
//       BX_CPUID_SVM_TSCRATE | // not implemented yet
//       BX_CPUID_SVM_VMCB_CLEAN_BITS | // not implemented yet
         BX_CPUID_SVM_FLUSH_BY_ASID |
//       BX_CPUID_SVM_DECODE_ASSIST | // not implemented yet
         BX_CPUID_SVM_PAUSE_FILTER;  // not implemented yet
//       BX_CPUID_SVM_PAUSE_FILTER_THRESHOLD; // not implemented yet
}
#endif

// leaf 0x00000000 //
void zambezi_t::get_std_cpuid_leaf_0(cpuid_function_t *leaf) const
{
  // EAX: highest std function understood by CPUID
  // EBX: vendor ID string
  // EDX: vendor ID string
  // ECX: vendor ID string
  get_leaf_0(0xD, "AuthenticAMD", leaf, 0x1);
}

// leaf 0x00000001 //
void zambezi_t::get_std_cpuid_leaf_1(cpuid_function_t *leaf) const
{
  // EAX:       CPU Version Information
  //   [3:0]   Stepping ID
  //   [7:4]   Model: starts at 1
  //   [11:8]  Family: 4=486, 5=Pentium, 6=PPro, ...
  //   [13:12] Type: 0=OEM, 1=overdrive, 2=dual cpu, 3=reserved
  //   [19:16] Extended Model
  //   [27:20] Extended Family
  leaf->eax = 0x00600F12;

  // EBX:
  //   [7:0]   Brand ID
  //   [15:8]  CLFLUSH cache line size (value*8 = cache line size in bytes)
  //   [23:16] Number of logical processors in one physical processor
  //   [31:24] Local Apic ID

  unsigned n_logical_processors = ncores*nthreads;
  leaf->ebx = ((CACHE_LINE_SIZE / 8) << 8) |
              (n_logical_processors << 16);
#if BX_SUPPORT_APIC
  leaf->ebx |= ((cpu->get_apic_id() & 0xff) << 24);
#endif

  // ECX: Extended Feature Flags
  // * [0:0]   SSE3: SSE3 Instructions
  // * [1:1]   PCLMULQDQ Instruction support
  //   [2:2]   DTES64: 64-bit DS area
  // * [3:3]   MONITOR/MWAIT support
  //   [4:4]   DS-CPL: CPL qualified debug store
  //   [5:5]   VMX: Virtual Machine Technology
  //   [6:6]   SMX: Secure Virtual Machine Technology
  //   [7:7]   EST: Enhanced Intel SpeedStep Technology
  //   [8:8]   TM2: Thermal Monitor 2
  // * [9:9]   SSSE3: SSSE3 Instructions
  //   [10:10] CNXT-ID: L1 context ID
  //   [11:11] reserved
  //   [12:12] FMA Instructions support
  // * [13:13] CMPXCHG16B: CMPXCHG16B instruction support
  //   [14:14] xTPR update control
  //   [15:15] PDCM - Perfmon and Debug Capability MSR
  //   [16:16] reserved
  //   [17:17] PCID: Process Context Identifiers
  //   [18:18] DCA - Direct Cache Access
  // * [19:19] SSE4.1 Instructions
  // * [20:20] SSE4.2 Instructions
  //   [21:21] X2APIC
  //   [22:22] MOVBE instruction
  // * [23:23] POPCNT instruction
  //   [24:24] TSC Deadline
  // * [25:25] AES Instructions
  // * [26:26] XSAVE extensions support
  // * [27:27] OSXSAVE support
  // * [28:28] AVX extensions support
  //   [29:29] AVX F16C - Float16 conversion support
  //   [30:30] RDRAND instruction
  //   [31:31] reserved
  leaf->ecx = get_std_cpuid_leaf_1_ecx();

  // EDX: Standard Feature Flags
  // * [0:0]   FPU on chip
  // * [1:1]   VME: Virtual-8086 Mode enhancements
  // * [2:2]   DE: Debug Extensions (I/O breakpoints)
  // * [3:3]   PSE: Page Size Extensions
  // * [4:4]   TSC: Time Stamp Counter
  // * [5:5]   MSR: RDMSR and WRMSR support
  // * [6:6]   PAE: Physical Address Extensions
  // * [7:7]   MCE: Machine Check Exception
  // * [8:8]   CXS: CMPXCHG8B instruction
  // * [9:9]   APIC: APIC on Chip
  //   [10:10] Reserved
  // * [11:11] SYSENTER/SYSEXIT support
  // * [12:12] MTRR: Memory Type Range Reg
  // * [13:13] PGE/PTE Global Bit
  // * [14:14] MCA: Machine Check Architecture
  // * [15:15] CMOV: Cond Mov/Cmp Instructions
  // * [16:16] PAT: Page Attribute Table
  // * [17:17] PSE-36: Physical Address Extensions
  //   [18:18] PSN: Processor Serial Number
  // * [19:19] CLFLUSH: CLFLUSH Instruction support
  //   [20:20] Reserved
  //   [21:21] DS: Debug Store
  //   [22:22] ACPI: Thermal Monitor and Software Controlled Clock Facilities
  // * [23:23] MMX Technology
  // * [24:24] FXSR: FXSAVE/FXRSTOR (also indicates CR4.OSFXSR is available)
  // * [25:25] SSE: SSE Extensions
  // * [26:26] SSE2: SSE2 Extensions
  //   [27:27] Self Snoop
  // * [28:28] Hyper Threading Technology
  //   [29:29] TM: Thermal Monitor
  //   [30:30] Reserved
  //   [31:31] PBE: Pending Break Enable
  leaf->edx = get_std_cpuid_leaf_1_edx();
#if BX_SUPPORT_SMP
  leaf->edx |= BX_CPUID_STD1_EDX_HT;
#endif
}

// leaf 0x00000005 MONITOR/MWAIT Leaf //
// leaf 0x00000006 Thermal and Power Management Leaf //
// leaf 0x0000000D XSAVE //

// leaf 0x80000000 //
void zambezi_t::get_ext_cpuid_leaf_0(cpuid_function_t *leaf) const
{
  get_leaf_0(0x8000001E, "AuthenticAMD", leaf);
}

// leaf 0x80000001 //
void zambezi_t::get_ext_cpuid_leaf_1(cpuid_function_t *leaf) const
{
  // EAX:       CPU Version Information (same as 0x00000001.EAX)
  leaf->eax = 0x00600f12;

  // EBX:
  //  [15:00]  Brand ID
  //  [27:16]  Reserved
  //  [31:28]  Package Type
  leaf->ebx = 0x10000000;

  // ECX:
  // * [0:0]   LAHF/SAHF instructions support in 64-bit mode
  // * [1:1]   CMP_Legacy: Core multi-processing legacy mode (AMD)
  // * [2:2]   SVM: Secure Virtual Machine (AMD)
  // * [3:3]   Extended APIC Space
  // * [4:4]   AltMovCR8: LOCK MOV CR0 means MOV CR8
  // * [5:5]   LZCNT: LZCNT instruction support
  // * [6:6]   SSE4A: SSE4A Instructions support
  // * [7:7]   Misaligned SSE support
  // * [8:8]   PREFETCHW: PREFETCHW instruction support
  // * [9:9]   OSVW: OS visible workarounds (AMD)
  // * [10:10] IBS: Instruction based sampling
  // * [11:11] XOP: Extended Operations Support and XOP Prefix
  // ! [12:12] SKINIT support
  // * [13:13] WDT: Watchdog timer support
  //   [14:14] Reserved
  // ! [15:15] LWP: Light weight profiling
  // * [16:16] FMA4: Four-operand FMA instructions support
  //   [17:17] Reserved
  //   [18:18] Reserved
  // * [19:19] NodeId: Indicates support for NodeId MSR (0xc001100c)
  //   [20:20] Reserved
  //   [21:21] TBM: trailing bit manipulation instructions support
  // * [22:22] Topology extensions support
  // * [23:23] PerfCtrExtCore: core perf counter extensions support
  // * [24:24] PerfCtrExtNB: NB perf counter extensions support
  //   [31:25] Reserved
  leaf->ecx = get_ext_cpuid_leaf_1_ecx(BX_CPUID_EXT1_ECX_CMP_LEGACY |
                                       BX_CPUID_EXT1_ECX_PREFETCHW |
                                       BX_CPUID_EXT1_ECX_OSVW |
                                       BX_CPUID_EXT1_ECX_IBS |
                                       BX_CPUID_EXT1_ECX_XOP |
                                       BX_CPUID_EXT1_ECX_WDT |
                                       BX_CPUID_EXT1_ECX_NODEID |
                                       BX_CPUID_EXT1_ECX_TOPOLOGY_EXTENSIONS |
                                       BX_CPUID_EXT1_ECX_PERFCTR_EXT_CORE |
                                       BX_CPUID_EXT1_ECX_PERFCTR_EXT_NB);

  // EDX:
  // Many of the bits in EDX are the same as FN 0x00000001 for AMD
  // * [0:0]   FPU on chip
  // * [1:1]   VME: Virtual-8086 Mode enhancements
  // * [2:2]   DE: Debug Extensions (I/O breakpoints)
  // * [3:3]   PSE: Page Size Extensions
  // * [4:4]   TSC: Time Stamp Counter
  // * [5:5]   MSR: RDMSR and WRMSR support
  // * [6:6]   PAE: Physical Address Extensions
  // * [7:7]   MCE: Machine Check Exception
  // * [8:8]   CXS: CMPXCHG8B instruction
  // * [9:9]   APIC: APIC on Chip
  //   [10:10] Reserved
  // * [11:11] SYSCALL/SYSRET support
  // * [12:12] MTRR: Memory Type Range Reg
  // * [13:13] PGE/PTE Global Bit
  // * [14:14] MCA: Machine Check Architecture
  // * [15:15] CMOV: Cond Mov/Cmp Instructions
  // * [16:16] PAT: Page Attribute Table
  // * [17:17] PSE-36: Physical Address Extensions
  //   [18:18] Reserved
  //   [19:19] Reserved
  // * [20:20] No-Execute page protection
  //   [21:21] Reserved
  // * [22:22] AMD MMX Extensions
  // * [23:23] MMX Technology
  // * [24:24] FXSR: FXSAVE/FXRSTOR (also indicates CR4.OSFXSR is available)
  // * [25:25] Fast FXSAVE/FXRSTOR mode support
  // * [26:26] 1G paging support
  // * [27:27] Support RDTSCP Instruction
  //   [28:28] Reserved
  // * [29:29] Long Mode
  //   [30:30] AMD 3DNow! Extensions
  //   [31:31] AMD 3DNow! Instructions
  leaf->edx = get_ext_cpuid_leaf_1_edx_amd();
}

// leaf 0x80000002 //
// leaf 0x80000003 //
// leaf 0x80000004 //

// leaf 0x80000005 //
void zambezi_t::get_ext_cpuid_leaf_5(cpuid_function_t *leaf) const
{
  leaf->eax = 0xff20ff18;
  leaf->ebx = 0xff20ff30;
  leaf->ecx = 0x10040140;
  leaf->edx = 0x40020140;
}

// leaf 0x80000006 //
void zambezi_t::get_ext_cpuid_leaf_6(cpuid_function_t *leaf) const
{
  leaf->eax = 0x64000000;
  leaf->ebx = 0x64004200;
  leaf->ecx = 0x08008140;
  leaf->edx = 0x0040c140;
}

// leaf 0x80000007 //
void zambezi_t::get_ext_cpuid_leaf_7(cpuid_function_t *leaf) const
{
  // CPUID function 0x80000007 - Advanced Power Management
  leaf->eax = 0;
  leaf->ebx = 0;
  leaf->ecx = 0;
  leaf->edx = 0x000003d9;
}

// leaf 0x80000008 //
void zambezi_t::get_ext_cpuid_leaf_8(cpuid_function_t *leaf) const
{
  bx_cpuid_t::get_ext_cpuid_leaf_8(leaf);

  // [17..16] Performance time-stamp counter size.
  // [15..12] APIC ID size, Zero value indicates that legacy methods must be
  //          used to derive the maximum number of cores.
  //  [11..8] (reserved)
  //   [7..0] Number of physical cores - 1.
  leaf->ecx = ncores - 1;
}

// leaf 0x80000009 : Reserved //

#if BX_SUPPORT_SVM

// leaf 0x8000000A : SVM //
void zambezi_t::get_ext_cpuid_leaf_A(cpuid_function_t *leaf) const
{
  leaf->eax = 0x01; /* SVM revision ID */
  leaf->ebx = 0x40; /* number of ASIDs */
  leaf->ecx = 0;

  // * [0:0]   NP - Nested paging support
  // * [1:1]   LBR virtualization
  // * [2:2]   SVM Lock
  // * [3:3]   NRIPS - Next RIP save on VMEXIT
  // * [4:4]   TscRate - MSR based TSC ratio control
  // * [5:5]   VMCB Clean bits support
  // * [6:6]   Flush by ASID support
  // * [7:7]   Decode assists support
  //   [8:8]   Reserved
  //   [9:9]   Reserved
  // * [10:10] Pause filter support
  //   [11:11] Reserved
  // * [12:12] Pause filter threshold support
  //   [13:13] Advanced Virtual Interrupt Controller
  leaf->edx = get_svm_extensions_bitmask();
}

#endif

// leaf 0x8000000B - 0x80000018: Reserved //

void zambezi_t::get_ext_cpuid_leaf_19(cpuid_function_t *leaf) const
{
  // CPUID function 0x800000019 - 1G Page TLB Identifiers
  leaf->eax = 0xF020F018;
  leaf->ebx = 0x64000000;
  leaf->ecx = 0;
  leaf->edx = 0;
}

// CPUID leaf 0x80000001A - Performance Optimization Identifiers

void zambezi_t::get_ext_cpuid_leaf_1B(cpuid_function_t *leaf) const
{
  // CPUID function 0x80000001B - Instruction Based Sampling Identifiers
  leaf->eax = 0xFF;
  leaf->ebx = 0;
  leaf->ecx = 0;
  leaf->edx = 0;

  BX_INFO(("WARNING: Instruction Based Sampling is not implemented"));
}

void zambezi_t::get_ext_cpuid_leaf_1C(cpuid_function_t *leaf) const
{
  // CPUID function 0x80000001C - Lightweight Profiling Capabilities (not implemented)
  leaf->eax = 0;
  leaf->ebx = 0;
  leaf->ecx = 0;
  leaf->edx = 0;
}

void zambezi_t::get_ext_cpuid_leaf_1D(Bit32u subfunction, cpuid_function_t *leaf) const
{
  // CPUID function 0x80000001D - Cache Properties
  switch(subfunction) {
  case 0:
    leaf->eax = 0x00000121;
    leaf->ebx = 0x00C0003F;
    leaf->ecx = 0x0000003F;
    leaf->edx = 0x00000000;
    return;
  case 1:
    leaf->eax = 0x00004122;
    leaf->ebx = 0x0040003F;
    leaf->ecx = 0x000001FF;
    leaf->edx = 0x00000000;
    return;
  case 2:
    leaf->eax = 0x00004143;
    leaf->ebx = 0x03C0003F;
    leaf->ecx = 0x000007FF;
    leaf->edx = 0x00000001;
    return;
  case 3:
    leaf->eax = 0x0000C163;
    leaf->ebx = 0x0FC0003F;
    leaf->ecx = 0x000007FF;
    leaf->edx = 0x00000001;
    return;
  default:
    leaf->eax = 0;
    leaf->ebx = 0;
    leaf->ecx = 0;
    leaf->edx = 0;
    return;
  }
}

void zambezi_t::get_ext_cpuid_leaf_1E(cpuid_function_t *leaf) const
{
  // CPUID function 0x80000001E - Topology Extensions
  leaf->eax = 0;
  leaf->ebx = (ncores - 1) << 8;
  leaf->ecx = 0;
  leaf->edx = 0;
}

void zambezi_t::dump_cpuid(void) const
{
  bx_cpuid_t::dump_cpuid(0xD, 0x8000001E);
}

bx_cpuid_t *create_zambezi_cpuid(BX_CPU_C *cpu) { return new zambezi_t(cpu); }

#endif
