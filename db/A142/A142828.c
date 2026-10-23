/*
 * A142828  Primes congruent to 30 mod 61.
 * https://oeis.org/A142828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 102, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 30 (mod 61) */
/* a(n) is the n-th prime p with p ≡ 30 (mod 61), n ≥ 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_generic.h"

seq_t a(idx_t n)
{
    /* Cached results for repeated calls. */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t  next_candidate;
    static bool   initialized = false;

    /* Initialise on first call.  The smallest prime ≡30 (mod 61) is 457. */
    if (!initialized) {
        cache_cap        = 16;
        cache_size       = 0;
        cache            = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        next_candidate   = 457ULL;      /* 30 + 61*7 */
        initialized      = true;
    }

    /* Grow the cache until it contains at least n terms. */
    while (cache_size < n) {
        if (is_prime_generic(next_candidate)) {
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        next_candidate += 61ULL;
    }

    return cache[(size_t)(n - 1)];
}
