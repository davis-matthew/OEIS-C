/*
 * A016225  Expansion of 1/((1-x)(1-4x)(1-10x)).
 * https://oeis.org/A016225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (50*10^n - 24*4^n + 1)/27
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t term1 = (seq_t)50 * ipow_u64_idx(10, n);
    seq_t term2 = (seq_t)24 * ipow_u64_idx(4, n);
    seq_t result = (term1 - term2 + 1) / (seq_t)27;
    return result;
}
