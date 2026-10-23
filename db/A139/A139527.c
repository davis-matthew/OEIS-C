/*
 * A139527  Numbers n such that numbers 24n+5 are primes.
 * https://oeis.org/A139527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation for OEIS A139527: 
 * numbers n such that 24*n+5 is prime (offset 1).
 * This implementation builds the sequence on demand, caching values
 * and testing candidates in order with is_prime().
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_cand = 0;    /* next candidate n to test */

    /* If we already have enough terms, return the requested one. */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Increase cache storage if needed. */
    size_t needed = (size_t)n;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) {
            new_cap = needed * 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* On real build this should be handled more gracefully. */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until the cache has at least n elements. */
    for (; cache_len < n; ++next_cand) {
        idx_t val = 24ULL * next_cand + 5ULL;  /* candidate prime to test */
        if (is_prime(val)) {
            cache[cache_len++] = (seq_t)next_cand;
        }
    }

    return cache[n - 1];
}
