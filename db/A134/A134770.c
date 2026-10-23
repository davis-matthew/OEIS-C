/*
 * A134770  a(n) = 4*A000984(n) - 3.
 * https://oeis.org/A134770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*C(2n,n) - 3
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute the central binomial coefficient C(2n,n) */
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t numerator = (seq_t)n + i;
        seq_t denom     = i;

        /* cancel common factors between numerator and denom */
        seq_t g = gcd(numerator, denom);
        numerator /= g;
        denom     /= g;

        /* cancel common factors between current binom and denom */
        g = gcd(binom, denom);
        binom /= g;
        denom /= g;

        binom = binom * numerator;
        binom = binom / denom;
    }

    /* a(n) = 4 * binom - 3 */
    seq_t result = binom * 4;
    result -= 3;
    return result;
}
