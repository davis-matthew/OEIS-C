/*
 * A142325  Primes congruent to 26 mod 45.
 * https://oeis.org/A142325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 272, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth prime ≡ 26 (mod 45) */
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Return quickly if we already have the value cached. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of primes stored */
    static size_t cache_cap  = 0;   /* capacity of cache */

    /* If requested index is already computed, return it. */
    if (cache && n <= cache_size)
        return cache[n - 1];

    /* Initialise cache on first use. */
    if (!cache) {
        cache_cap  = 64;
        cache_size = 0;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failed – should not happen in tests */
    }

    /* Start examining candidates from the last found prime + 45, or 71. */
    idx_t cand = (cache_size > 0) ? (idx_t)cache[cache_size - 1] + 45 : 71;

    /* Keep searching until we have at least n primes. */
    while (cache_size < n) {
        if (is_prime(cand)) {
            /* Grow the cache if necessary. */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                if (new_cap < n * 2)
                    new_cap = n * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return 0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)cand;
        }
        cand += 45; /* next candidate ≡ 26 mod 45 */
    }

    return cache[n - 1];
}
