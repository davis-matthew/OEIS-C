/*
 * A164072  a(n) = 8*a(n-1) - 14*a(n-2) for n > 1; a(0) = 1, a(1) = 7.
 * https://oeis.org/A164072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 14*a(n-2), with a(0) = 1, a(1) = 7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoised values */
static seq_t *cache = NULL;
static idx_t cache_max;   /* last cached index (valid after cache init) */

/* Ensure the cache contains values up to index n.  */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initialise with the first two terms */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache_max = 1;
    }

    if (n <= cache_max) return;

    /* grow the cache to hold indices up to n */
    idx_t new_size = n + (idx_t)1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;

    /* compute new terms using the recurrence relation */
    for (idx_t i = cache_max + (idx_t)1; i <= n; ++i) {
        cache[i] = (seq_t)8 * cache[i - (idx_t)1]
                 - (seq_t)14 * cache[i - (idx_t)2];
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
