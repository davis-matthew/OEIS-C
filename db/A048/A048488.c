/*
 * A048488  a(n) = 6*2^n - 5.
 * https://oeis.org/A048488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*2^n - 5
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_pow = pow2(n);          /* 2^n  */
    seq_t res     = two_pow * (seq_t)6; /* 6*2^n */
    res -= (seq_t)5;                 /* 6*2^n - 5 */
    return res;
}
