/*
 * A137483  a(n+1) = 9*a(n) - 6, a(0) = 2.
 * https://oeis.org/A137483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(n+1) = 9*a(n) - 6 (OEIS A137483)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache to memoise computed values across arbitrary call order */
static seq_t *cache = NULL;          /* pointer to cached terms */
static idx_t cache_size = 0;         /* number of initialized terms in cache */
static size_t cache_cap = 0;         /* allocated capacity of cache */

/* Compute the n-th term of A137483, 0‑indexed. */
seq_t a(idx_t n)
{
    /* initialise cache with term 0 if it doesn't exist yet */
    if (cache == NULL) {
        cache_cap   = 1;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]    = (seq_t)2;   /* a(0) = 2 */
        cache_size  = 1;
    }

    /* if requested n is already cached, return it immediately */
    if (n < cache_size) {
        return cache[n];
    }

    /* grow the cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) {
            new_cap = n + 1;
        }
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failed – abort with zero (unlikely in tests) */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* recurrence: a(i) = 9*a(i-1) - 6 */
        cache[i] = cache[i - 1] * (seq_t)9 - (seq_t)6;
    }
    cache_size = n + 1;

    return cache[n];
}
