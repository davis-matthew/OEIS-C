/*
 * A016170  Expansion of 1/((1-6*x)*(1-8*x)).
 * https://oeis.org/A016170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*8^n - 3*6^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 948 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t pow8 = ipow_u64_idx(8, n);
    seq_t pow6 = ipow_u64_idx(6, n);
    return pow8 * (seq_t)4 - pow6 * (seq_t)3;
}
