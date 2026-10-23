/*
 * A109344  a(n) consists of n 4's, n-1 8's and a single 9 (in that order).
 * https://oeis.org/A109344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (4*10^(2n) + 4*10^n + 1)/9, n ≥ 1
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 10^n and 10^(2n) are provided by pow10.  Compute
       (4*10^(2n) + 4*10^n + 1) / 9, which is always an integer. */
    seq_t term = (seq_t)4 * pow10(2 * n) + (seq_t)4 * pow10(n) + (seq_t)1;
    return term / (seq_t)9;
}
