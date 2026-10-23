/*
 * A176281  Hankel transform of A176280.
 * https://oeis.org/A176281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 16*a(n-2) + 8*a(n-3) with initial values
 *            a(0)=1, a(1)=3, a(2)=12 (this is the Hankel transform of
 *            A176280)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free, NULL, size_t */

static seq_t *cache      = NULL;   /* cache of computed values */
static size_t cache_len  = 0;      /* number of valid entries in cache */
static size_t cache_cap  = 0;      /* allocated capacity of cache */

/* enlarge cache to hold indices up to n (inclusive) and compute new terms */
static void ensure_cache(idx_t n)
{
    /* grow capacity if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)n)
            new_cap *= 2;

        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_arr == NULL) {
            /* allocation failure: leave cache unchanged */
            return;
        }
        cache     = new_arr;
        cache_cap = new_cap;
    }

    /* initialise missing initial values if cache empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)12;
        cache_len = 3;
    } else if (cache_len < 3) {
        if (cache_len < 1) cache[0] = (seq_t)1;
        if (cache_len < 2) cache[1] = (seq_t)3;
        if (cache_len < 3) cache[2] = (seq_t)12;
        cache_len = 3;
    }

    /* compute terms from cache_len up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i < 3)  /* already initialised */
            continue;
        cache[i] = (seq_t)8 * cache[i-1]
               - (seq_t)16 * cache[i-2]
               + (seq_t)8  * cache[i-3];
    }
    if (cache_len < (size_t)n + 1)
        cache_len = (size_t)n + 1;
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* Return trivially for n < 3 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)12;

    /* Ensure cache is up to date */
    if (cache_len <= (size_t)n)
        ensure_cache(n);

    return cache[n];
}
