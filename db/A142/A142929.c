/*
 * A142929  Primes congruent to 9 mod 64.
 * https://oeis.org/A142929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 200, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 9 mod 64.
   Uses on‑demand trial division with caching. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t num); */

seq_t a(idx_t n) {
    /* static cache for the sequence values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next candidate (congruent to 9 mod 64) to test */
    static uint64_t next_candidate = 73;

    /* initialise on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 0;
        next_candidate = 73;
    }

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        if (is_prime_small(next_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 64;   /* maintain 9 mod 64 sequence */
    }

    return cache[n - 1];
}
