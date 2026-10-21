/*
 * A224856  Numbers n such that 90*n + 29 and 90*n + 31 are twin primes.
 * https://oeis.org/A224856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers n such that 90*n+29 and 90*n+31 are twin primes.
   The function returns the n‑th such integer (a(1) = 0). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cache of discovered terms (stored as idx_t to avoid big‑int
       allocation when the sequence values are small). */
    static idx_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    /* Next integer to test. */
    static idx_t next_candidate = 0;

    /* Ensure enough space in the cache. */
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Find terms until we have at least `n` of them. */
    while (cache_len < n) {
        /* Compute the twin prime numbers to test. */
        seq_t p1 = 90 * (seq_t)next_candidate + 29;
        seq_t p2 = p1 + 2;

        if (is_prime_big(p1) && is_prime_big(p2))
            cache[cache_len++] = next_candidate;

        ++next_candidate;
    }

    /* Return the requested term. */
    return (seq_t)cache[n - 1];
}
