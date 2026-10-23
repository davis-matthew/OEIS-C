/*
 * A153877  Numbers n such that 5n-1 and 5n+1 are both prime.
 * https://oeis.org/A153877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 222, u32 n <= 248, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A153877: Numbers n such that 5n-1 and 5n+1 are both prime.
   This implementation keeps a cache of all found terms and extends it
   lazily when a requested index n is larger than the current cache size.
   It works for any seq_t (arbitrary‐precision or fixed width) and is
   O(n) in the number of returned terms. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static state shared across calls */
    static seq_t *cache = NULL;      /* array of found n's */
    static size_t cache_size = 0;    /* number of stored terms      */
    static size_t cache_cap  = 0;    /* allocated capacity          */
    static idx_t  next_candidate = 6;/* next multiple of 6 to try  */

    /* Return immediately if the requested value is already cached. */
    if (cache_size >= n)
        return cache[n - 1];

    /* Lazy allocation: start with a modest capacity. */
    if (!cache_cap) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_size < n) {
        seq_t cand = (seq_t)next_candidate;
        seq_t p1   = cand * (seq_t)5 - (seq_t)1;
        seq_t p2   = cand * (seq_t)5 + (seq_t)1;

        /* Test primality of 5n-1 and 5n+1. */
        if (is_prime_big(p1) && is_prime_big(p2)) {
            /* Grow the cache if needed. */
            if (cache_size == cache_cap) {
                cache_cap = cache_cap * 2 + 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = cand;
        }

        /* Move to the next multiple of 6. */
        next_candidate += 6;
    }

    return cache[n - 1];
}
