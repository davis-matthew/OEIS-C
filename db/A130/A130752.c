/*
 * A130752  Binomial transform of periodic sequence (2, 3, 1).
 * https://oeis.org/A130752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(1)=5, a(2)=9, a(n)=3*a(n-1)-3*a(n-2)+2*a(n-3) for n>2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Cached values for memoisation across calls */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;  /* number of elements stored (0 .. cache_len-1) */
static idx_t  cache_cap = 0;  /* current capacity of the cache */

/* Grow the cache to accommodate indices up to 'need' inclusive */
static void ensure_capacity(idx_t need)
{
    if (need < cache_cap) return;
    /* double size until we reach needed capacity */
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= need) new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort(); /* allocation failed – nothing sane can be done */
    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache     = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Initialise cache with first three terms if empty */
    if (cache_len == 0) {
        ensure_capacity(2);                /* need space for indices 0,1,2 */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)9;
        cache_len = 3;
    }

    if (n < cache_len) return cache[n];

    /* Extend cache to reach n */
    ensure_capacity(n);

    /* Compute missing terms iteratively */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (seq_t)3 * cache[i - 1]
                    - (seq_t)3 * cache[i - 2]
                    + (seq_t)2 * cache[i - 3];
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
