/*
 * A118405  Row sums of triangle A118404.
 * https://oeis.org/A118405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) - a(n-2) - 2*a(n-3) with initial values a(0)=1,
 *            a(1)=0, a(2)=0
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 33, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n‑th term of OEIS A118405. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache   = NULL;  /* allocated array of values */
    static size_t cache_len = 0;   /* number of values currently computed   */
    static size_t cache_cap = 0;   /* allocated capacity (in elements)   */

    /* Resize the cache if needed, then fill new entries. */
    if ((size_t)n >= cache_len) {
        size_t need = (size_t)n + 1;
        if (need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap : 8;
            while (new_cap < need)
                new_cap <<= 1;
            /* realloc returns void*, cast explicitly for C++ */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();               /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute all missing values in order. */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1 || i == 2) {
                cache[i] = (seq_t)0;
            } else {
                /* a(i) = -(2*a(i-1) + a(i-2) + 2*a(i-3)) */
                seq_t t1 = (seq_t)2 * cache[i - 1];
                seq_t t2 = cache[i - 2];
                seq_t t3 = (seq_t)2 * cache[i - 3];
                cache[i] = -(t1 + t2 + t3);
            }
        }
        cache_len = need;
    }

    return cache[(size_t)n];
}
