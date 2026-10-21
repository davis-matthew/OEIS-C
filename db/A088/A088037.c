/*
 * A088037  Smallest square k == 1 (mod some n-th power), k > 1.
 * https://oeis.org/A088037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 (n=1), 9 (n=2), (2^n-2)^2/4 (n>2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 1)   return (seq_t)4;
    if (n == 2)   return (seq_t)9;

    /* compute 2^n */
    seq_t two_n = pow_seq_t((seq_t)2, n);

    /* (2^n - 2) */
    seq_t diff = two_n - (seq_t)2;

    /* square */
    seq_t sq = diff * diff;

    /* divide by 4 (exact) */
    return sq / (seq_t)4;
}
