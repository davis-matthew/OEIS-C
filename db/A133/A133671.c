/*
 * A133671  a(n) = a(n-1) - 64*a(n-2), a(0)=1, a(1)=8.
 * https://oeis.org/A133671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - 64*a(n-2), a(0) = 1, a(1) = 8
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 10, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache      = NULL;
static idx_t  cache_cap  = 0;   /* capacity in elements */
static idx_t  cache_used = 0;   /* number of computed elements */

/* Expand the cache to hold at least (n+1) elements. */
static void
ensure_cache(idx_t n)
{
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? (size_t)cache_cap * 2 : 16;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        /* Cast the returned pointer to satisfy C++ compilation. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Ensure the buffer is large enough for indices up to n. */
    ensure_cache(n);

    /* Initialise the first terms if they are not already set. */
    if (cache_used == 0) {
        cache[0] = (seq_t)1;
        if (n >= 1)
            cache[1] = (seq_t)8;
        cache_used = (n >= 1 ? 2 : 1);
    } else if (cache_used == 1) {
        cache[1] = (seq_t)8;
        cache_used = 2;
    }

    /* If the requested term is already known, return it. */
    if (n < cache_used)
        return cache[n];

    /* Compute missing terms up to n inclusive. */
    for (idx_t i = cache_used; i <= n; ++i) {
        cache[i] = cache[i - 1] - (seq_t)64 * cache[i - 2];
    }
    cache_used = n + 1;
    return cache[n];
}
