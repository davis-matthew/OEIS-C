/*
 * A136505  a(n) = binomial(2^n + 1, n).
 * https://oeis.org/A136505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2^n + 1, n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* N = 2^n + 1 */
    seq_t N = pow2(n);    /* pow2 returns 2^n as seq_t */
    N += (seq_t)1;

    seq_t res = (seq_t)1;

    /* Compute binomial(N, n) multiplicatively, reducing at each step
       to keep intermediate values as small as possible. */
    for (idx_t i = (idx_t)0; i < n; ++i)
    {
        seq_t numer = N - (seq_t)i;          /* N - i = N - (i) */
        seq_t denom = (seq_t)(i + (idx_t)1); /* i + 1 */

        /* Reduce numerator and denominator by their gcd */
        seq_t g = gcd(denom, numer);
        denom /= g;
        numer /= g;

        /* Further reduce any remaining denominator factor by res */
        g = gcd(res, denom);
        res /= g;
        denom /= g;  /* denom should now be 1 */

        /* Multiply the result by the reduced numerator */
        res *= numer;
    }

    return res;
}
