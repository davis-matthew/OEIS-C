/*
 * A121842  Difference between n^3 and next prime.
 * https://oeis.org/A121842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = next_prime(n³) – n³ * (i.e., the difference between n³
 *            and the smallest prime larger than n³)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2147, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Compute n³ in 64‑bit to avoid overflow for n ≤ 10 000. */
    uint64_t cube64 = (uint64_t)n * (uint64_t)n * (uint64_t)n;

    /* Find the next prime > cube64. */
    uint64_t p = cube64 + 1;
    while (!is_prime_small(p)) {
        ++p;
    }

    /* Return the difference as seq_t.  For larger seq_t types the cast
     * yields the exact value; for smaller types the result simply
     * overflows as required by the specification. */
    return (seq_t)(p - cube64);
}
