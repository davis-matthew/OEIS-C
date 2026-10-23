/*
 * A145121  Numbers n such that there exists x in N : (x+19)^3-x^3=n^2.
 * https://oeis.org/A145121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A145121: a(1)=361, a(2)=108661,
   recurrence: a(n+2) = 302*a(n+1) - a(n)   (n ≥ 1) */

#include <stdlib.h>   /* for malloc, realloc, abort, free */
#ifdef __cplusplus
#include <new>       /* placement new for C++ builds */
#endif

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cacheSize = 0;   /* number of terms currently stored */

    /* initialisation on first call:  a(1) and a(2) */
    if (cacheSize == 0) {
#ifdef __cplusplus
        cache = new seq_t[2];
#else
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
#endif
        if (!cache) abort();
        cache[0] = (seq_t)361;        /* a(1) */
        cache[1] = (seq_t)108661;     /* a(2) */
        cacheSize = 2;
    }

    /* grow the cache to satisfy the requested index */
    if (n > cacheSize) {
        idx_t newSize = n;
#ifdef __cplusplus
        /* allocate a new array and copy old values */
        seq_t *newCache = new seq_t[newSize];
        for (idx_t i = 0; i < cacheSize; ++i)
            newCache[i] = cache[i];
        /* free old storage */
        delete[] cache;
#else
        /* C-style reallocation */
        seq_t *newCache = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (!newCache) abort();
        newCache[cacheSize] = 0; /* initialise the new element(s) */
#endif
        cache = newCache;

        /* compute missing terms */
        for (idx_t i = cacheSize; i < newSize; ++i)
            cache[i] = (seq_t)302 * cache[i - 1] - cache[i - 2];

        cacheSize = newSize;
    }

    /* return the requested term (a(1) is at cache[0]) */
    return cache[n - 1];
}
