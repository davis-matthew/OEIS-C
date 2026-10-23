/*
 * A105371  Expansion of (1-x)*(1-x+x^2)/(1-3*x+4*x^2-2*x^3+x^4).
 * https://oeis.org/A105371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3*a(n-1)-4*a(n-2)+2*a(n-3)-a(n-4),
 * Range:     i8 n <= 10, i16 n <= 23, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, abort */

/* initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=0 */

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated elements */
    static idx_t cache_size = 0;  /* computed elements */

    /* --------- ensure capacity for at least n+1 elements --------- */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : (idx_t)8;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* --------- initialise first four terms if not yet done --------- */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)0;
        cache_size = 4;
    }

    /* --------- compute missing terms up to n using recurrence --------- */
    if (n + 1 > cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] =
                (seq_t)3 * cache[i - 1]
                - (seq_t)4 * cache[i - 2]
                + (seq_t)2 * cache[i - 3]
                -       cache[i - 4];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
