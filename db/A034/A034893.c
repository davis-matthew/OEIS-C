/*
 * A034893  Maximum of different products of partitions of n into distinct
 *          parts.
 * https://oeis.org/A034893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (b + k)!/(b!*(b + k - r)) for n ≥ 2 where k = ⌊(√(8*n +
 *            9) - 1)/2⌋, d = n - k*(k+1)/2 + 1, b = ⌊d/(k-1)⌋ + 1, r = d mod
 *            (k-1). For n = 0,1, a(n) = 1
 * Range:     u8 n <= 17, u16 n <= 38, u32 n <= 89, u64 n <= 227,
 *            u128 n <= 598, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Please include the integer square‑root helper provided in the build. */
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Compute k using an integer square‑root */
    idx_t k = (int_sqrt(8*n + 9) - 1) / 2;        /* k ≥ 2 for n ≥ 2 */

    /* Compute d, b, r, and the upper index B = b + k */
    idx_t d   = n - k*(k+1)/2 + 1;
    idx_t denom = k - 1;                          /* k-1 > 0 */
    idx_t b = d / denom + 1;
    idx_t r = d % denom;
    idx_t B = b + k;

    /* The factor to skip is C = b + k - r = B - r */
    idx_t C = B - r;

    /* Compute product of (b+1)…B, excluding the factor C */
    seq_t result = (seq_t)1;
    for (idx_t i = b + 1; i <= B; ++i) {
        if (i == C)
            continue;        /* skip the denominator factor */
        result *= (seq_t)i;
    }

    return result;
}
