/*
 * A034273  a(n) = binomial(2*n+6,n+7)*(n^2+7*n+1)/(n+8) = f(n,n+6) where f is
 *          given in A034261.
 * https://oeis.org/A034273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n+6, n+7) * (n^2+7*n+1)/(n+8) (offset 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* trivially 0 when the binomial is undefined */
    if (n == 0)
        return (seq_t)0;

    seq_t result = (seq_t)1;
    idx_t i, k = n + 7;               /* k = n+7 is the length of the binomial product */
    for (i = 1; i <= k; ++i)
    {
        /* numerator term of binomial(2*n+6, n+7) */
        seq_t num = (seq_t)(n - 1 + i);   /* n-1+i runs from n to 2n+6 */

        /* cancel the current denominator i with the running result first */
        seq_t g1 = gcd(result, (seq_t)i);
        result /= g1;
        seq_t denom = (seq_t)i / g1;      /* remaining part of denominator */

        /* cancel the remaining denominator with the current numerator */
        seq_t g2 = gcd(num, denom);
        num /= g2;
        denom /= g2;                      /* denom should now be 1 */

        result *= num;                    /* safely multiply */
    }

    /* multiply by the polynomial factor (n^2 + 7*n + 1) */
    seq_t poly = (seq_t)n * (seq_t)n + (seq_t)7 * (seq_t)n + (seq_t)1;

    /* final division by (n+8); reduce carefully to avoid overflow */
    seq_t denom = (seq_t)(n + 8);
    seq_t g3 = gcd(result, denom);
    result /= g3;
    denom /= g3;

    seq_t g4 = gcd(poly, denom);
    poly /= g4;
    denom /= g4;                          /* denom should now be 1 */

    result *= poly;

    /* any remaining denominator (should be 1) is divided here as a safety net */
    if (denom != (seq_t)1)
        result /= denom;

    return result;
}
