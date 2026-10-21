/*
 * A077902  Expansion of (1-x)^(-1)/(1 + x - x^2 + x^3).
 * https://oeis.org/A077902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 36, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A077902 – Expansion of (1-x)^(-1) / (1 + x - x^2 + x^3)
   Recurrence: a(0)=1, a(1)=0, a(2)=2, a(3)=-2
                a(n) = 2*a(n-2) - 2*a(n-3) + a(n-4)   (n ≥ 4)   */

/* The function is memoised; it works for any seq_t (signed integer or
   arbitrary‑precision class) and any idx_t, provided idx_t is
   unsigned (as in the build). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of elements currently stored */

    if ((idx_t)(n + 1) > cache_size) {
        /* enlarge the cache to hold indices up to n */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0) exit(1);    /* allocation failed */
        cache = tmp;

        /* initialise the low‑index terms that may not yet exist */
        if (cache_size == 0) {
            if (new_size > 0) cache[0] = (seq_t)1;
            if (new_size > 1) cache[1] = (seq_t)0;
            if (new_size > 2) cache[2] = (seq_t)2;
            if (new_size > 3) cache[3] = (seq_t)(-2);
        }
        else if (cache_size < 4) {
            if (cache_size <= 1 && new_size > 1) cache[1] = (seq_t)0;
            if (cache_size <= 2 && new_size > 2) cache[2] = (seq_t)2;
            if (cache_size <= 3 && new_size > 3) cache[3] = (seq_t)(-2);
        }

        /* compute missing terms using the recurrence */
        idx_t start = cache_size;
        if (start < 4) start = 4;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = (seq_t)(2 * cache[i-2]
                               - 2 * cache[i-3]
                               + cache[i-4]);
        }

        cache_size = new_size;
    }

    return cache[n];
}
