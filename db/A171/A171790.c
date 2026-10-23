/*
 * A171790  G.f. A(x) satisfies: A(x*(1+x)^3) = 1 + x.
 * https://oeis.org/A171790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3*(-1)^(n-1)*binom(4n-1, n-1)/(4n-1), a(0)=1
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;            /* a(0) = 1 */

    seq_t res = (seq_t)1;                   /* will hold C(4n-1, n-1) */
    /* compute binomial coefficient multiplicatively with GCD reductions */
    for (idx_t j = 1; j <= n-1; ++j) {
        seq_t numer = (seq_t)(4*n - j);     /* numerator (4n - j)   */
        seq_t denom = (seq_t)j;             /* denominator j        */

        /* Reduce numerator/denominator */
        seq_t g = gcd(numer, denom);
        numer /= g;
        denom /= g;

        /* Reduce res/denominator */
        g = gcd(res, denom);
        res /= g;
        denom /= g;

        /* multiply and divide */
        res *= numer;
        res /= denom;
    }

    /* a(n) = 3*(-1)^(n-1) * res / (4n-1) */
    res *= (seq_t)3;
    res /= (seq_t)(4*n - 1);

    /* apply sign (-1)^(n-1) */
    seq_t sign = ((n-1) & 1) ? -(seq_t)1 : (seq_t)1;
    res *= sign;

    return res;
}
