/*
 * A060790  Inscribe two circles of curvature 2 inside a circle of curvature
 *          -1. Sequence gives curvatures of the smallest circles that can be
 *          sequentially inscribed in such a diagram.
 * https://oeis.org/A060790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4) Initial values:
 *            a(0) = -1, a(1) = 2, a(2) = 2, a(3) = 3
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 21, i64 n <= 42, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to support arbitrary call order */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of terms stored (up to len-1) */

    /* Ensure the cache can hold at least indices 0..n */
    if (cache == NULL) {
        /* First call: allocate minimum 4 entries for initial values */
        cap = (n + 1 > 4) ? (size_t)(n + 1) : 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();           /* fatal if memory allocation fails */

        /* Initialize the first four terms */
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t) 2;
        cache[2] = (seq_t) 2;
        cache[3] = (seq_t) 3;
        len     = 4;
    }
    else if ((size_t)(n + 1) > cap) {
        /* Grow the cache to accommodate larger indices */
        size_t new_cap = cap * 2;
        if ((size_t)(n + 1) > new_cap) new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* fatal if realloc fails */
        cache = tmp;
        cap   = new_cap;
    }

    /* If the requested term is already computed, return it */
    if (n < len) return cache[n];

    /* Compute terms up to n using the recurrence */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t v = (seq_t)(2 * cache[i-1] + 2 * cache[i-2] + 2 * cache[i-3] - cache[i-4]);
        cache[i] = v;
    }
    len = (size_t)n + 1;

    return cache[n];
}
