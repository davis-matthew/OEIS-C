/*
 * A158623  Denominator of the reduced fraction A158620(n)/A158621(n).
 * https://oeis.org/A158623
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = denominator of the reduced fraction product_{k=2..n}
 *            (k^3-1)/(k^3+1) == denominator of 2*(n^2+n+1)/(3*n*(n+1))
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2.
       For smaller n we return 1 as a benign placeholder. */
    if (n < 2)
        return (seq_t)1;

    /* Compute the numerator and denominator of the simplified fraction:
       N = 2*(n^2 + n + 1)
       D = 3*n*(n+1)     */
    seq_t sn = (seq_t)n;
    seq_t n_sq = sn * sn;                /* n^2            */
    seq_t N = (seq_t)2 * (n_sq + sn + (seq_t)1);
    seq_t D = (seq_t)3 * sn * (sn + (seq_t)1);

    /* Reduce by the gcd and return the remaining denominator. */
    seq_t g = gcd(N, D);
    return D / g;
}
