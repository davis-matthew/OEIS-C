/*
 * A162772  a(n) = ((2+sqrt(5))*(4+sqrt(5))^n + (2-sqrt(5))*(4-sqrt(5))^n)/2.
 * https://oeis.org/A162772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A162772: a(n) = ((2+√5)*(4+√5)^n + (2-√5)*(4-√5)^n)/2
 * Recurrence: a(0)=2, a(1)=13, a(n)=8*a(n-1)-11*a(n-2) for n≥2      */
#include <stdlib.h>
#include <stdint.h>

#if defined(__cplusplus)
    #include <new>
#endif

seq_t a(idx_t n)
{
    /* Cache for already computed values – allows arbitrary call order. */
    static size_t    cap   = 0;          /* allocated capacity          */
    static size_t    size  = 0;          /* number of stored terms      */
#if defined(__cplusplus)
    static seq_t*    cache = nullptr;
#else
    static seq_t*    cache = NULL;
#endif

    /* Initialise the cache with the first two terms. */
    if (cache == NULL) {
        cap   = 2;
#if defined(__cplusplus)
        cache = new seq_t[cap];
#else
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
#endif
        if (!cache) abort();           /* out‑of‑memory; abort safely   */
        cache[0] = (seq_t)2;           /* a(0)                          */
        cache[1] = (seq_t)13;          /* a(1)                          */
        size   = 2;
    }

    /* Grow the cache if it cannot hold term n. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap > 0 ? cap * 2 : 16;
        while ((size_t)n >= new_cap) new_cap *= 2;
#if defined(__cplusplus)
        seq_t* new_cache = new seq_t[new_cap];
#else
        seq_t* new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        if (!new_cache) abort();
        for (size_t i = 0; i < size; ++i) new_cache[i] = cache[i];
#if defined(__cplusplus)
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute missing terms up to n (inclusive). */
    for (size_t i = size; i <= (size_t)n; ++i) {
        if (i < 2) continue;           /* already initialised          */
        /* a(i) = 8*a(i-1) - 11*a(i-2) */
        cache[i] = (seq_t)8 * cache[i-1] - (seq_t)11 * cache[i-2];
    }

    /* Update stored size to the largest index known. */
    if ((size_t)n + 1 > size) size = (size_t)n + 1;

    return cache[n];
}
