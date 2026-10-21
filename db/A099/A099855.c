/*
 * A099855  a(n) = n*2^n - 2^(n/2)*sin(Pi*n/4).
 * https://oeis.org/A099855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 14*a(n-2) + 16*a(n-3) - 8*a(n-4) initial
 *            values: a(0)=0, a(1)=1, a(2)=6, a(3)=22
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute OEIS sequence A099855 */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid entries in cache */
    static size_t cache_cap = 0;   /* allocated capacity of cache */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 8;                                 /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        /* guard against out‑of‑memory (in practice this won't happen) */
        if (!cache) return (seq_t)0;

        /* known initial values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)22;
        cache_len = 4;
    }

    /* direct return for already known indices */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* ensure enough space for all up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) {
            /* fallback: keep old cache if allocation fails */
            new_cap = cache_cap;   /* no space, but we cannot compute more */
        } else {
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* compute missing values using the linear recurrence */
    for (size_t i = cache_len; i <= n; ++i) {
        seq_t val =
            6 * cache[i - 1]
            - 14 * cache[i - 2]
            + 16 * cache[i - 3]
            - 8 * cache[i - 4];
        cache[i] = val;
    }

    cache_len = needed;
    return cache[(size_t)n];
}
