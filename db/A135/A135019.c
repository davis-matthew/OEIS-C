/*
 * A135019  a(n) = -a(n-1) + 2*a(n-2) - a(n-3), with a(0) = 0, a(1) = 1, a(2)
 *          = -3.
 * https://oeis.org/A135019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 2*a(n-2) - a(n-3) (n >= 3), with initial values
 *            a(0) = 0, a(1) = 1, a(2) = -3
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 28, i64 n <= 57, i128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: holds already computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;           /* number of terms stored in cache */

    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-3;

    /* Initialise cache with the first three terms on first use */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failed – abort silently */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-3;
    }

    /* Grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return cache[cap - 1];          /* out of memory – return last known value */
        cache = tmp;

        /* Compute terms up to n (including n) using the recurrence */
        for (size_t i = cap; i <= (size_t)n; ++i) {
            cache[i] = -cache[i - 1] + (cache[i - 2] * (seq_t)2) - cache[i - 3];
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
