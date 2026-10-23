/*
 * A138341  Expansion of (1-4x-x^3)/(1-x+x^2)^2.
 * https://oeis.org/A138341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) + 2*a(n-3) - a(n-4); a(0)=1, a(1)=-2,
 *            a(2)=-7, a(3)=-7 (Recurrence is valid for n > 3)
 * Range:     i8 n <= 61, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to store computed values for any order of calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;            /* Number of elements stored */

    /* If we need to compute up to n, grow the cache */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* Compute all missing entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)       cache[0] = 1;
            else if (i == 1)  cache[1] = -2;
            else if (i == 2)  cache[2] = -7;
            else if (i == 3)  cache[3] = -7;
            else {
                /* The recurrence uses only integer arithmetic */
                cache[i] = 2 * cache[i-1]      /* 2*a(n-1) */
                         - 3 * cache[i-2]      /* 3*a(n-2) */
                         + 2 * cache[i-3]      /* 2*a(n-3) */
                         -     cache[i-4];     /* a(n-4) */
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
