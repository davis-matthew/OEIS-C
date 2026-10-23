/*
 * A018091  Expansion of 1/((1-3*x)*(1-9*x)*(1-11*x)).
 * https://oeis.org/A018091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 159*a(n-2) + 297*a(n-3); with a(0)=1,
 *            a(1)=23, a(2)=370
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Return explicitly for the first three terms. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)23;
    if (n == 2) return (seq_t)370;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cacheSize = 0;            /* allocated size of cache */
    static idx_t lastComputed = 2;         /* highest n whose value is stored */
    static bool initialized = false;       /* flag for first allocation */

    /* Allocate or grow the cache on first use. */
    if (!initialized) {
        /* Start with a modest size; grow by doubling as needed. */
        cacheSize = (idx_t)8;
        if (n + 1 > cacheSize) {
            cacheSize = n + 1;
        }
        cache = (seq_t *)malloc(sizeof(seq_t) * cacheSize);
        if (!cache) return (seq_t)0; /* allocation failure: fall back to zero */

        /* Initialize known first values. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)23;
        cache[2] = (seq_t)370;
        for (idx_t i = 3; i < cacheSize; ++i) cache[i] = (seq_t)0;

        initialized = true;
    }
    else if (n + 1 > cacheSize) {
        /* Grow the cache to accommodate the requested index. */
        idx_t newSize = cacheSize;
        while (newSize <= n) newSize <<= 1;  /* double until large enough */
        seq_t *newCache = (seq_t *)realloc(cache, sizeof(seq_t) * newSize);
        if (!newCache) return (seq_t)0;      /* allocation failure? */
        cache = newCache;
        /* Initialise newly allocated part to zero. */
        for (idx_t i = cacheSize; i < newSize; ++i) cache[i] = (seq_t)0;
        cacheSize = newSize;
    }

    /* Compute any missing terms up to n. */
    if (lastComputed < n) {
        for (idx_t i = lastComputed + 1; i <= n; ++i) {
            cache[i] =   (seq_t)23  * cache[i-1]
                       - (seq_t)159 * cache[i-2]
                       + (seq_t)297 * cache[i-3];
        }
        lastComputed = n;
    }

    return cache[n];
}
