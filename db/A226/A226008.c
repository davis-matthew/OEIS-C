/*
 * A226008  a(0) = 0; for n>0, a(n) = denominator(1/4 - 4/n^2).
 * https://oeis.org/A226008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator(1/4 - 4/n^2)
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t denom = (seq_t)4 * nn;             /* 4*n^2 */

    /* absolute value of n^2 - 16 without overflow */
    seq_t num_abs = (nn >= (seq_t)16) ? nn - (seq_t)16 : (seq_t)16 - nn;

    seq_t g = gcd(num_abs, denom);           /* gcd of numerator and denominator */
    return denom / g;                        /* reduced denominator */
}
