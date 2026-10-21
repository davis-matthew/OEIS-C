/*
 * A054514  Number of ways to place non-crossing diagonals in convex (n+4)-gon
 *          so as to create no triangles or quadrilaterals.
 * https://oeis.org/A054514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..floor((n-1)/3)} binomial(n-2*j-1, n-3*j-1) *
 *            binomial(n+3+j, n+2) / (n+3) Implements the OEIS sequence
 *            A054514 using the provided binomial() and gcd() utilities
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    seq_t sum = 0;
    idx_t max_j = (n - 1) / 3;

    for (idx_t j = 0; j <= max_j; ++j)
    {
        idx_t a1 = n - 2 * j - 1;
        idx_t b1 = n - 3 * j - 1;
        seq_t bin1 = binomial(a1, b1);

        idx_t a2 = n + 3 + j;
        idx_t b2 = n + 2;
        seq_t bin2 = binomial(a2, b2);

        seq_t denom = (seq_t)n + 3;

        /* cancel common factors with the denominator to avoid overflow */
        seq_t g1 = gcd(bin1, denom);
        bin1 /= g1;
        denom /= g1;

        seq_t g2 = gcd(bin2, denom);
        bin2 /= g2;
        denom /= g2;   /* should become 1 because the product is divisible */

        seq_t term = bin1 * bin2;   /* denom == 1 */
        sum += term;
    }

    return sum;
}
