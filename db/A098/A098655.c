/*
 * A098655  Trace sequence of 3 X 3 symmetric Krawtchouk matrix.
 * https://oeis.org/A098655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3, a(1)=2, a(2)=20,
   a(n)=2*a(n-1)+8*a(n-2)-16*a(n-3) for n≥3 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored                  */
    static idx_t cache_alloc = 0; /* allocated capacity (>= cache_len)       */

    /* Initialise the cache with the first three terms. */
    if (cache_len == 0)
    {
        cache_alloc = 3;
        cache = (seq_t*)malloc(cache_alloc * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed               */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)20;
        cache_len = 3;
    }

    /* If requested index is already cached, return it. */
    if (n < cache_len)
        return cache[n];

    /* Ensure the cache has enough space for indices up to n. */
    idx_t needed = n + 1;
    if (needed > cache_alloc)
    {
        idx_t new_alloc = cache_alloc;
        while (new_alloc < needed) new_alloc <<= 1;    /* double until large enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                    /* reallocation failed       */
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len; i < needed; ++i)
    {
        cache[i] = (seq_t)2 * cache[i-1]                 /* 2*a(i-1) */
                 + (seq_t)8 * cache[i-2]                /* 8*a(i-2) */
                 - (seq_t)16 * cache[i-3];              /* 16*a(i-3) */
    }
    cache_len = needed;
    return cache[n];
}
