/*
 * A041263  Denominators of continued fraction convergents to sqrt(143).
 * https://oeis.org/A041263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041263: a(n) = 24*a(n-2) - a(n-4)
   with a(0)=1, a(1)=1, a(2)=23, a(3)=24. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache   = NULL;   /* array of terms */
    static size_t cache_cap = 0;     /* allocated size */
    static idx_t  max_known  = 0;    /* highest index computed */

    /* ---- ensure the array has room for index n ---- */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed – return 0 */
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* ---- initialise the first four terms if not yet set ---- */
    if (cache[0] == 0) {
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)23;  /* a(2) */
        cache[3] = (seq_t)24;  /* a(3) */
        max_known = 3;
    }

    /* ---- compute any missing terms up to n ---- */
    for (idx_t i = max_known + 1; i <= n; ++i) {
        /* recurrence holds for i >= 4 */
        if (i >= 4)
            cache[i] = (seq_t)24 * cache[i - 2] - cache[i - 4];
    }
    max_known = n;

    return cache[n];
}
