/*
 * A159337  Transform of the finite sequence (1, 0, -1, 0, 1) by the T_{1,0}
 *          transformation (see link).
 * https://oeis.org/A159337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=4, a(3)=11, a(4)=27, * a(5)=61, a(6)=141, *
 *            a(n)=3*a(n-1)-2*a(n-2)+a(n-3) for n≥7
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Allocate or grow the cache to hold indices up to n. */
static void ensure_cache(idx_t n, seq_t **cache, size_t *alloc)
{
    size_t needed = (size_t)n + 1;
    if (*alloc >= needed) return;

    size_t newsize = (*alloc) ? (*alloc) : 1;
    while (newsize < needed) newsize <<= 1;

    seq_t *tmp = (seq_t *)realloc(*cache, newsize * sizeof(seq_t));
    if (!tmp) exit(1);                /* Out of memory: abort */
    *cache = tmp;
    *alloc = newsize;
}

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_alloc = 0;       /* size of allocated memory */
    static idx_t  cache_last = 6;        /* largest index whose value is known */

    /* Initialise cache on first entry */
    if (cache == NULL) {
        ensure_cache(6, &cache, &cache_alloc);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)27;
        cache[5] = (seq_t)61;
        cache[6] = (seq_t)141;
        cache_last = 6;
    }

    /* If n is already computed, return it */
    if ((idx_t)n <= cache_last)
        return cache[(size_t)n];

    /* Ensure the buffer is large enough for indices up to n */
    ensure_cache(n, &cache, &cache_alloc);

    /* Compute missing terms up to n */
    for (size_t i = cache_last + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)((seq_t)3 * cache[i-1]
                          - (seq_t)2 * cache[i-2]
                          + cache[i-3]);
    }

    cache_last = n;
    return cache[(size_t)n];
}
