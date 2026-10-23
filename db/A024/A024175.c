/*
 * A024175  Expansion of g.f. (1 - 5*x + 6*x^2 - x^3)/((1 - 2*x)*(1 - 4*x +
 *          2*x^2)).
 * https://oeis.org/A024175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) + 4*a(n-3) for n >= 4; a(0) = 1,
 *            a(1) = 1, a(2) = 2, a(3) = 5
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;

    /* Static cache to store computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_sz  = 0;

    /* Initialise the cache on the first call. */
    if (cache == NULL) {
        cache_cap = 4;
        cache_sz  = 4;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!cache) abort();          /* memory allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
    }

    /* If the requested index is already cached, return it. */
    if ((size_t)n < cache_sz) {
        return cache[(size_t)n];
    }

    /* Resize the cache if necessary. */
    size_t target = (size_t)n + 1;                  /* we need indices 0..n inclusive */
    if (target > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < target) new_cap <<= 1;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        /* copy existing values */
        for (size_t i = 0; i < cache_sz; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();                  /* memory reallocation failure */
        cache = new_cache;
#endif
        cache_cap = new_cap;
    }

    /* Compute new terms until we reach the desired index. */
    for (size_t i = cache_sz; i < target; ++i) {
        seq_t v1 = cache[i - 1];
        seq_t v2 = cache[i - 2];
        seq_t v3 = cache[i - 3];
        /* recurrence: a(n) = 6*a(n-1) - 10*a(n-2) + 4*a(n-3) */
        cache[i] = v1 * 6 - v2 * 10 + v3 * 4;
    }
    cache_sz = target;

    return cache[(size_t)n];
}
