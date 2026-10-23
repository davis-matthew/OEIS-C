/*
 * A142779  Primes congruent to 52 mod 59.
 * https://oeis.org/A142779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 52 (mod 59)
 * Range:     u8 n <= 1, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1; return 0 for invalid indices */

    /* Static cache of found primes */
    static seq_t *cache   = NULL;   /* array holding primes ≡ 52 (mod 59) */
    static size_t cache_len = 0;    /* number of primes stored */
    static size_t cache_cap = 0;    /* allocated capacity */
    static uint64_t next_candidate = 52;   /* next number congruent to 52 mod 59 */
    static bool initialized = false;      /* whether the cache has been set up */

    /* Lazy initialization of the cache array */
    if (!initialized) {
        cache_cap = 16;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache)   /* memory allocation failure – should not happen */
            return (seq_t)0;
        initialized = true;
    }

    /* Grow the cache until we have at least n primes recorded */
    while (cache_len < (size_t)n) {
        /* Expand capacity if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache)   /* memory allocation failure – bail out */
                return (seq_t)0;
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Test the current candidate for primality */
        if (is_prime((idx_t)next_candidate)) {
            cache[cache_len] = (seq_t)next_candidate;
            cache_len++;
        }

        /* Advance to the next number that is 52 (mod 59) */
        next_candidate += 59;
    }

    /* n is 1‑indexed; cache is 0‑indexed */
    return cache[(size_t)n - 1];
}
