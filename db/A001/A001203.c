/*
 * A001203  Simple continued fraction expansion of Pi.
 * https://oeis.org/A001203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, cofr
 * Formula:   no closed form. a(n) is the n-th partial quotient of the simple
 *            continued fraction of Pi. The code computes Pi with big-integer
 *            arithmetic using Machin's formula, Pi = 16*arctan(1/5) -
 *            4*arctan(1/239), and tracks the rounding error so it has exact
 *            rational bounds lo < Pi < hi. It then runs Euclid...
 * Range:     u8 n <= 3, u16 n <= 9856, u32 n <= 9856, u64 n <= 9856,
 *            u128 n <= 9856, big any n
 * Helpers:   pcf_addmul, pcf_addsmall, pcf_atan, pcf_bits, pcf_cmp, pcf_divs,
 *            pcf_ext, pcf_limb, pcf_norm, pcf_run, pcf_step, pcf_submul,
 *            pcf_subn, pcf_subsmall, pcf_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pcf_addmul.h"
#include "../utils/pcf_addsmall.h"
#include "../utils/pcf_atan.h"
#include "../utils/pcf_bits.h"
#include "../utils/pcf_cmp.h"
#include "../utils/pcf_divs.h"
#include "../utils/pcf_ext.h"
#include "../utils/pcf_limb.h"
#include "../utils/pcf_norm.h"
#include "../utils/pcf_run.h"
#include "../utils/pcf_step.h"
#include "../utils/pcf_submul.h"
#include "../utils/pcf_subn.h"
#include "../utils/pcf_subsmall.h"
#include "../utils/pcf_zero.h"

static uint64_t *pcf_terms = NULL;
static size_t pcf_cnt = 0;

static void pcf_compute(size_t N, idx_t need)
{
    size_t L = (4 * N + 256) / 32 + 2;
    for (;;) {
        uint64_t *terms;
        size_t cnt = pcf_run(L, &terms);
        if (cnt > pcf_cnt) {
            free(pcf_terms);
            pcf_terms = terms;
            pcf_cnt = cnt;
        } else {
            free(terms);
        }
        if ((idx_t)pcf_cnt > need) break;
        L += L / 2;
    }
}

seq_t a(idx_t n)
{
    if (n >= (idx_t)pcf_cnt) {
        size_t N = (size_t)n + 1;
        if (N < 2 * pcf_cnt) N = 2 * pcf_cnt;
        pcf_compute(N, n);
    }
    return (seq_t)pcf_terms[n];
}
