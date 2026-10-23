/*
 * A153059  a(0) = 0, a(n) = a(n-1)^2 - n.
 * https://oeis.org/A153059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0, a(n) = a(n-1)^2 - n (for n > 0)
 * Range:     i8 n <= 6, i16 n <= 7, i32 n <= 8, i64 n <= 9, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t size   = 0;     /* number of allocated entries   */
    static idx_t last   = 0;     /* highest index that has been computed */

    /* initialise cache on first call */
    if (!cache) {
        size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;            /* a(0) */
    }

    /* expand cache if necessary */
    if (n >= size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        size  = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = prev * prev - (seq_t)i;
    }
    last = n;

    return cache[n];
}
