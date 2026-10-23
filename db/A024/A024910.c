/*
 * A024910  Numbers k such that 9*k - 2 is prime.
 * https://oeis.org/A024910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 9*k - 2 is prime. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/*  Returns the n-th integer k with 9*k - 2 prime,
    for any n >= 1.  The function caches results so that
    subsequent calls are O(1) after the first search.  */
seq_t a(idx_t n)
{
    /* Persistent cache of the found terms. */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    /* Next candidate k to test. */
    static idx_t next_candidate = 1;

    /* Lazy allocation of the cache. */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        idx_t k           = next_candidate;
        idx_t candidate   = 9 * k - 2;       /* 9*k - 2 */
        if (is_prime(candidate)) {
            /* Grow cache if needed. */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)k;
        }
        ++next_candidate;
    }

    return cache[(size_t)(n - 1)];
}
