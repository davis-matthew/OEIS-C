/*
 * A178061  Number of distinct cycles without repeated edges on the multigraph
 *          consisting of two vertices joined by n edges.
 * https://oeis.org/A178061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   a(n) = n*(n-1)/2 + Sum_{k=4..n, 2|k} n! / ((n-k)! * 2*k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute one term of the sum by canceling the denominator as soon as possible
   to avoid overflow for the cases where the result still fits into seq_t. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Cycles of length 2 */
    if (n >= 2)
        result = (seq_t)n * (seq_t)(n - 1) / 2;

    /* Even‑length cycles (k ≥ 4) */
    for (idx_t k = 4; k <= n; k += 2)
    {
        seq_t term = 1;
        seq_t denom = (seq_t)2 * (seq_t)k;          /* start denominator */

        for (idx_t i = n - k + 1; i <= n; ++i)
        {
            /* cancel common factors between current factor and remaining denominator */
            seq_t g = gcd(denom, (seq_t)i);
            denom /= g;
            seq_t mul = (seq_t)i / g;

            /* multiply, the product is guaranteed to stay within seq_t for the
               values that fit (n ≤ ~21 for 64‑bit seq_t). */
            term *= mul;

            /* after multiplication, cancel any remaining part of the denominator
               that has become a divisor of the new term. */
            if (denom > 1)
            {
                seq_t g2 = gcd(term, denom);
                if (g2 > 1)
                {
                    term /= g2;
                    denom /= g2;
                }
            }
        }
        /* after the loop denom should be 1 for a valid integer term */
        result += term;
    }

    return result;
}
