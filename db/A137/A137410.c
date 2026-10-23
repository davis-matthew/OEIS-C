/*
 * A137410  a(n) = (5^n - 3)/2.
 * https://oeis.org/A137410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*a(n-1)+6, a(0) = -1 (i.e. a(n) = (5^n‑3)/2)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 27, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t last = 0;   /* highest index currently filled (>=0) */
    static size_t cap = 0;   /* allocated capacity */

    /* Ensure cache has space for index n */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        /* Allocate or grow the cache */
        if (cap == 0) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        cap = new_cap;
    }

    /* Initialize base case if not yet done */
    if (last == 0 && (cap > 0)) {
        cache[0] = (seq_t)-1;
    }

    /* Compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)5 + (seq_t)6;
    }
    last = n;

    return cache[n];
}
