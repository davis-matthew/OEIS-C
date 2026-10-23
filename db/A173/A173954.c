/*
 * A173954  a(n) = denominator of (Zeta(2, 3/4) - Zeta(2, n-1/4)), where Zeta
 *          is the Hurwitz Zeta function.
 * https://oeis.org/A173954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of Σ_{k=0}^{n-2} 1/(4k+3)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* A rational sum is maintained as num/den in reduced form.
       For each term 1/(4k+3)^2 the sum is updated:
           num/den + 1/d   ->   new_num/new_den
       where d = (4k+3)^2. */
    seq_t num = 0;
    seq_t den = 1;

    /* No terms for n < 2 – the empty sum is 0/1. */
    if (n <= 1)
        return den;

    /* Loop over k = 0 .. n-2  (n-1 terms). */
    for (idx_t i = 0; i < n - 1; ++i)
    {
        /* Compute d = (4*i + 3)^2 using seq_t to avoid overflow of idx_t. */
        seq_t t = (seq_t)i;
        t = 4 * t + 3;
        seq_t d = t * t;

        /* Use gcd to form the least common multiple of den and d. */
        seq_t g1   = gcd(den, d);
        seq_t den_g = den / g1;
        seq_t d_g   = d   / g1;

        /* New denominator: den_g * d   (lcm(den, d) = den/g1 * d). */
        seq_t new_den = den_g * d;

        /* New numerator: num * (d/g1) + (den/g1). */
        seq_t new_num = num * d_g + den_g;

        /* Reduce the fraction again. */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return den;
}
