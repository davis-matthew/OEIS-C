/*
 * A097118  Expansion of (1-x)/((1-x)^2 - 5*x^3).
 * https://oeis.org/A097118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 5*a(n-3), a(0) = 1, a(1) = 1, a(2) =
 *            1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t size = 0;        /* number of terms computed */

    /* initialise cache with first three terms */
    if (size == 0) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        /* safety check for build errors */
        if (!cache) exit(1);
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        size = 3;
    }

    /* Return already computed value if possible */
    if (n < size) return cache[n];

    /* extend cache to hold term n */
    size_t new_size = (size_t)n + 1;   /* new size needed */
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!cache) exit(1);              /* allocation failure */

    /* compute missing terms using recurrence
       (loop invariant: terms up to size-1 are known) */
    for (idx_t i = size; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] - cache[i - 2] + (seq_t)5 * cache[i - 3];
    }
    size = new_size;
    return cache[n];
}
