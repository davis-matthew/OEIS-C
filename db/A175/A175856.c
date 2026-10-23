/*
 * A175856  a(n) = a(n-1) - 1 if n is composite, a(n) = n otherwise.
 * https://oeis.org/A175856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is 1 or prime; a(n) = a(n-1) – 1 for composite n.
 *            Initial value: a(1) = 1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* array of computed a(i) */
    static size_t cache_cap = 0;     /* capacity of cache (indices 0 .. cache_cap-1) */
    static idx_t cache_max = 0;      /* last index for which we have a value */

    if (!cache) {
        /* Allocate indices 0 and 1 (index 0 unused).  cache[1] = 1. */
        cache_cap = 2;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();
        cache[1] = (seq_t)1;
        cache_max = 1;
    }

    /* If request exceeds current capacity, grow the cache. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;          /* indices 0 .. n */
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        /* Zero the new part to keep values defined. */
        memset(new_cache + cache_cap, 0,
               (new_cap - cache_cap) * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1 || is_prime(i))
            cache[i] = (seq_t)i;          /* noncomposite */
        else
            cache[i] = cache[i - 1] - (seq_t)1; /* composite */
    }
    if (n > cache_max) cache_max = n;

    return cache[n];
}
