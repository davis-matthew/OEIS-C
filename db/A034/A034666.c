/*
 * A034666  Sum of n-th powers of divisors of 36.
 * https://oeis.org/A034666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 2^n + 4^n) * (1 + 3^n + 9^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 193 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Compute the required powers */
    seq_t p2 = ipow_u64_idx(2, n);
    seq_t p4 = ipow_u64_idx(4, n);
    seq_t p3 = ipow_u64_idx(3, n);
    seq_t p9 = ipow_u64_idx(9, n);

    /* Compute the sums */
    seq_t s1 = 1 + p2 + p4;
    seq_t s2 = 1 + p3 + p9;

    /* Return the product */
    return s1 * s2;
}
