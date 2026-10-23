/*
 * A105748  Number of ways to use the elements of {1,..,k}, 0<=k<=2n, once
 *          each to form a collection of n (possibly empty) sets, each with at
 *          most 2 elements.
 * https://oeis.org/A105748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*a(n-1) - (2*n-2)*a(n-2) - a(n-3); Initial values:
 *            a(0)=1, a(1)=3, a(2)=10
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence is exact for all n; the implementation
   caches computed terms and grows the cache as needed. */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_malloc.h"

seq_t a(idx_t n)
{
    /* cache holds computed sequence values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the first three terms */
    if (!cache) {
        cache_size = 3;
        cache = seq_malloc(cache_size);
        if (!cache) return 0;          /* unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)10;
    }

    /* if requested term is already cached, return it */
    if (n < cache_size) return cache[n];

    /* enlarge cache to accommodate terms up to n */
    idx_t new_size = n + 1;
    seq_t *new_cache = seq_malloc(new_size);
    if (!new_cache) return 0;          /* allocation failure (unlikely) */
    memcpy(new_cache, cache, cache_size * sizeof(seq_t));
    free(cache);
    cache = new_cache;
    idx_t old_size = cache_size;
    cache_size = new_size;

    /* compute missing terms using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        /* i >= 3 here because old_size is at least 3 after init */
        seq_t term1 = cache[i-1] * (seq_t)(2 * i);
        seq_t term2 = cache[i-2] * (seq_t)(2 * i - 2);
        cache[i] = term1 - term2 - cache[i-3];
    }

    return cache[n];
}
