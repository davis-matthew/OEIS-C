/*
 * A227712  a(n) = 9*2^n - 3*n - 5.
 * https://oeis.org/A227712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*2^n - 3*n - 5
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p = pow2(n);                 /* 2^n */
    return (seq_t)9 * p - (seq_t)3 * n - (seq_t)5;
}
