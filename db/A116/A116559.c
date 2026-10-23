/*
 * A116559  Expansion of g.f.
 *          x*(1+x+2*x^2+2*x^3+5*x^4+5*x^5-3*x^6+2*x^7-x^8-x^9)/(1-6*x^6-x^12).
 * https://oeis.org/A116559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 6 * a(n-6) + a(n-12) (n >= 12), * with initial values: *
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=2, a(5)=5, * a(6)=5,
 *            a(7)=3, a(8)=8, a(9)=11, a(10)=11, a(11)=30 * (this recurrence
 *            was given in the OEIS entry)
 * Range:     u8 n <= 19, u16 n <= 37, u32 n <= 74, u64 n <= 148,
 *            u128 n <= 292, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;     /* pointer to stored terms */
    static size_t cache_cap = 0;    /* allocated array size */
    static size_t cache_max = 0;    /* first index not yet filled */

    /* initialise cache if first call */
    if (cache == NULL) {
        /* we store at least 13 terms (0..12); this will be expanded later */
        const size_t init_cap = 13;
#ifdef __cplusplus
        cache = new seq_t[init_cap];
#else
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* out of memory */
        memset(cache, 0, init_cap * sizeof(seq_t));   /* zero‑initialize */
#endif
        /* copy the integer literals into the cache */
        static const seq_t init_terms[13] = {
            (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)2,
            (seq_t)5, (seq_t)5, (seq_t)3, (seq_t)8, (seq_t)11,
            (seq_t)11, (seq_t)30, (seq_t)30
        };
        for (size_t i = 0; i < init_cap; ++i)
            cache[i] = init_terms[i];
        cache_cap = init_cap;
        cache_max = init_cap;   /* 0 .. 12 are valid */
    }

    /* request index as size_t for array access */
    const size_t idx = (size_t)n;

    /* enlarge cache if necessary */
    if (idx >= cache_cap) {
        const size_t new_cap = idx + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
#else
        new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        if (new_cache == NULL) return 0;   /* out of memory */

        /* copy existing values to the new larger array */
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];

        /* free the old storage */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to idx (recurrence: n >= 12) */
    if (idx >= cache_max) {
        for (size_t i = cache_max; i <= idx; ++i) {
            if (i >= 12) {
                cache[i] = (seq_t)6 * cache[i-6] + cache[i-12];
            }
        }
        cache_max = idx + 1;
    }

    return cache[(size_t)n];
}
