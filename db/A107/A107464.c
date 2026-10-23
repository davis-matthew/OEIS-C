/*
 * A107464  Number of fuzzy subgroups of rank 3 cyclic group of order
 *          (p^n)*q*r where p, q and r are three distinct prime.
 * https://oeis.org/A107464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n+1)*(n^2 + 6*n + 6) - 1
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t inner = nn * nn + (seq_t)6 * nn + (seq_t)6;

    seq_t pow2 = (seq_t)1;
    /* compute 2^(n+1) by multiplying by 2, n+1 times */
    for (idx_t i = 0; i <= n; ++i)
        pow2 *= (seq_t)2;

    seq_t result = pow2 * inner;
    result -= (seq_t)1;
    return result;
}
