/*
 * A179744  a(0) = 1, a(n) = 3*2^(n-1) - n for n>0.
 * https://oeis.org/A179744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-1) - n for n>0, a(0) = 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t val = pow2(n - 1);        /* 2^(n-1) */
    val = val * (seq_t)3;           /* 3*2^(n-1) */
    val = val - (seq_t)n;           /* subtract n */
    return val;
}
