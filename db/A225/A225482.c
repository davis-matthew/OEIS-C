/*
 * A225482  Expansion of 1/(1 - x^3 - x^4 - x^5 + x^8).
 * https://oeis.org/A225482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 49, u32 n <= 94, u64 n <= 184,
 *            u128 n <= 363, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-3) + a(n-4) + a(n-5) – a(n-8)
   Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=1, a(6)=1, a(7)=2 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;          /* pointer to allocated array */
    static idx_t cache_size  = 0;        /* allocated size (indices)   */
    static idx_t cache_max   = 0;        /* largest index computed      */

    /* Ensure the cache can hold the requested index */
    if (cache_size <= n) {
        size_t new_size = cache_size ? cache_size << 1 : 8;
        size_t need     = (size_t)n + 1;
        if (new_size < need) new_size = need;

        seq_t *tmp;
#ifdef __cplusplus
        /* In C++ use operator new[]; copy old values if any */
        tmp = new seq_t[new_size];
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i)
                tmp[i] = cache[i];
            delete[] cache;
        }
#else
        /* In C use realloc */
        tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
#endif
        cache      = tmp;
        cache_size = (idx_t)new_size;
    }

    /* Initialise the first eight terms once */
    if (cache_max < 7) {
        if (!cache) {
            cache = (seq_t *)malloc(8 * sizeof(seq_t));
            if (!cache) abort();
            cache_size = 8;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)1;
        cache[6] = (seq_t)1;
        cache[7] = (seq_t)2;
        cache_max = 7;
    }

    /* Return already computed values */
    if (n <= cache_max)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 3) val += cache[i - 3];
        if (i >= 4) val += cache[i - 4];
        if (i >= 5) val += cache[i - 5];
        if (i >= 8) val -= cache[i - 8];
        cache[i] = val;
    }
    cache_max = n;
    return cache[n];
}
