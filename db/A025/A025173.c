/*
 * A025173  The Gegenbauer Polynomial of index n, order 1, evaluated at x=1/3
 *          and multiplied by n*3^n/2.
 * https://oeis.org/A025173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=-5, a(3)=-42,
 *            a(n)=4*a(n-1)-22*a(n-2)+36*a(n-3)-81*a(n-4) for n>=4
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 17, i64 n <= 37, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already-computed terms */
    static seq_t *cache = NULL;
    static size_t   cache_len = 0;      /* number of terms currently stored */

    /* initialise cache with first four terms if necessary */
    if (cache == NULL) {
        size_t init_cap = 4;
        cache = (seq_t*)malloc(sizeof(seq_t)*init_cap);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = -5;
        cache[3] = -42;
        cache_len = 4;
    }

    /* extend cache up to n if needed */
    if ((size_t)n >= cache_len) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cache_len * 2)
            new_cap = cache_len * 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));

        for (size_t k = cache_len; k <= (size_t)n; ++k) {
            /* use rotation of last 4 terms to compute next term */
            seq_t v = 4 * cache[k-1] - 22 * cache[k-2] + 36 * cache[k-3] - 81 * cache[k-4];
            cache[k] = v;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
