/*
 * A072525  a(0) = 1; a(n+1) is smallest composite number > a(n) such that
 *          a(n) + a(n+1) is prime.
 * https://oeis.org/A072525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n+1) = smallest composite number > a(n) such that a(n)+a(n+1) is prime.
   Initial values: a(0) = 1. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Local helper to grow the cache array when needed. */
static void grow_cache(seq_t **array, size_t *cap, size_t new_cap) {
    seq_t *tmp = (seq_t *)realloc(*array, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failed: terminate */
    *array = tmp;
    *cap   = new_cap;
}

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache  = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;   /* number of elements stored (≥1) */

    /* initialise cache with a(0) if needed */
    if (cache == NULL) {
        cache_cap = 8;                      /* initial capacity */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                /* allocation failed */
        cache_len = 1;                      /* we already have a(0) */
        cache[0]  = (seq_t)1;               /* a(0) */
    }

    /* generate terms up to index n */
    while (cache_len <= n) {
        seq_t cur = cache[cache_len - 1];

        /* find next term */
        seq_t candidate = cur + (seq_t)1;   /* start with first number > cur */
        for (;;) {
            /* check composite: >1 and not prime */
            if (candidate > (seq_t)1 && !is_prime_big(candidate)) {
                seq_t sum = cur + candidate;
                if (is_prime_big(sum)) {
                    /* found a(n+1) */
                    break;
                }
            }
            candidate++;   /* try next number */
        }

        /* ensure capacity for new element */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            grow_cache(&cache, &cache_cap, new_cap);
        }

        cache[cache_len] = candidate;
        cache_len++;
    }

    return cache[n];
}
