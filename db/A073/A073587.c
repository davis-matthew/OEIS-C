/*
 * A073587  a(n) = a(n-1)*2^n + 1 where a(0)=1.
 * https://oeis.org/A073587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) * 2^n + 1, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Computes sequence A073587: a(0) = 1, a(n) = a(n-1)*2^n + 1 */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (>=1 after init) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 8; /* initial generous capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();            /* allocation failure */
        cache[0] = (seq_t)1; /* a(0) = 1 */
        cache_len = 1;
    }

    /* Extend cache up to requested index n, if needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        if (new_len > cache_cap) {
            /* grow capacity (at least double, but not less than new_len) */
            idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
            if (new_cap < new_len) new_cap = new_len;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (cache[i - 1] << i) + (seq_t)1;
        }
        cache_len = new_len;
    }

    return cache[n];
}
