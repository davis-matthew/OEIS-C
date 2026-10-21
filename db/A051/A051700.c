/*
 * A051700  Distance from n to closest prime that is different from n.
 * https://oeis.org/A051700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   distance to closest prime that is different from n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

/* Compute the OEIS sequence A051700: distance from n to the closest prime
 * other than n.  For n < 2 the distance is to the smallest prime (2).
 * For prime n, the distance is to the nearest other prime. */
seq_t a(idx_t n)
{
    if (n < 2)
    {
        /* 0 -> 2, 1 -> 1 */
        return (seq_t)(2 - n);
    }

    idx_t d = 1;
    while (1)
    {
        /* Check downwards */
        if (n > d)
        {
            idx_t lower = n - d;
            if (lower >= 2 && is_prime(lower))
                return (seq_t)d;
        }
        /* Check upwards */
        if (is_prime(n + d))
            return (seq_t)d;

        d++;
        /* Safe guard: avoid overflow on very large n */
        if (n == 0 || d > n + 1)
            return (seq_t)0; /* Should never happen for finite n */
    }
}
