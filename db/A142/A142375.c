/*
 * A142375  Primes congruent to 24 mod 47.
 * https://oeis.org/A142375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 24 (mod 47)
 * Range:     u8 n <= 1, u16 n <= 141, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Prime test supplied by the build system. */
#include "../utils/is_prime.h"

/* Cached values of the sequence.
   The cache is filled lazily and can be accessed in any order. */
static seq_t *cache      = NULL; /* array of stored primes */
static size_t cache_len  = 0;    /* number of primes currently cached */
static size_t cache_cap  = 0;    /* allocated capacity of the array */
static idx_t  next_candidate = 0;/* next number to test (always 24 mod 47) */
static bool   initialized   = false;/* whether the cache has been set up */

/* Return the n-th member of the sequence (n≥1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for the offset, but safe */

    /* Initialise the cache on the first call. */
    if (!initialized) {
        cache_cap = 64;                    /* start with a small array */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_candidate = 71;               /* first prime ≡24 (mod 47) */
        initialized = true;
    }

    /* Generate primes until we have at least n terms. */
    while (cache_len < n) {
        idx_t cand = next_candidate;

        /* Find the next prime in the arithmetic progression 24 + 47 k. */
        while (!is_prime(cand))
            cand += 47;

        /* Grow the cache if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failure – give up */
                free(cache);
                cache = NULL;
                cache_len = cache_cap = 0;
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the found prime. */
        cache[cache_len++] = (seq_t)cand;

        /* Next candidate to start searching from. */
        next_candidate = cand + 47;
    }

    return cache[n - 1];
}
