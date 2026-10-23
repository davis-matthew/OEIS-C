/*
 * A122948  First row sum of the 5 X 5 matrix M^n, where M = {{0, -1, 0, 0,
 *          0}, {-1, 1, -1, 0, 0}, {0, -1, 1, -1, 0}, {0, 0, -1, 1, -1}, {0,
 *          0, 0, -1, 1}}, n>=0.
 * https://oeis.org/A122948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 5*a(n-3) + 2*a(n-4) + a(n-5) with
 *            initial values a(0)=1, a(1)=-1, a(2)=1, a(3)=-1, a(4)=1
 * Range:     i8 n <= 9, i16 n <= 14, i32 n <= 26, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of filled elements in cache */
    static size_t cache_cap = 0; /* capacity of cache */

    /* initial base values */
    static const seq_t base[5] = {1, -1, 1, -1, 1};

    /* initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 32;                    /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* store the first five terms */
        for (size_t i = 0; i < 5; ++i)
            cache[i] = base[i];
        cache_len = 5;
    }

    /* if requested index already cached, return it */
    if ((size_t)n < cache_len)
        return cache[n];

    /* compute terms up to n using recurrence */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        /* grow cache if necessary */
        if ((size_t)i >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) { /* allocation failure, abort */
                /* In contest environment abort or return zero */
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* recurrence: a(i) = 4*a(i-1) - 2*a(i-2) - 5*a(i-3) + 2*a(i-4) + a(i-5) */
        seq_t val = 4 * cache[i-1]           /* 4*a(i-1) */
                  - 2 * cache[i-2]           /* -2*a(i-2) */
                  - 5 * cache[i-3]           /* -5*a(i-3) */
                  + 2 * cache[i-4]           /* +2*a(i-4) */
                  +  cache[i-5];             /* +a(i-5) */

        cache[i] = val;
        ++cache_len;
    }

    return cache[n];
}
