/*
 * A028917  a(n) = (3*n+1)! / (24*n).
 * https://oeis.org/A028917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)! / (24*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Denominator 24 will be cancelled during the loop. */
    seq_t denom = (seq_t)24;
    seq_t result = (seq_t)1;

    idx_t limit = 3 * n + 1;
    for (idx_t i = 1; i <= limit; ++i) {
        /* Cancel the factor n exactly once. */
        if (i == n) continue;

        seq_t factor = (seq_t)i;
        seq_t g = gcd(factor, denom);
        if (g) {
            denom  = denom / g;
            factor = factor / g;
        }

        result = result * factor;
    }

    /* Any remaining denominator (should be 1) is divided now. */
    if (denom != (seq_t)1) {
        result = result / denom;
    }

    return result;
}
