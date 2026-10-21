/*
 * A051699  Distance from n to closest prime.
 * https://oeis.org/A051699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = distance from n to the nearest prime number
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the minimal distance between n and the closest prime number. */
seq_t a(idx_t n)
{
    for (idx_t d = 0; ; ++d) {
        /* Check a prime below (or equal to) n if the subtraction remains valid. */
        if (n >= d && is_prime(n - d))
            return (seq_t)d;
        /* Check a prime above (or equal to) n. */
        if (is_prime(n + d))
            return (seq_t)d;
    }
}
