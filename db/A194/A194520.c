/*
 * A194520  First coordinate of (3,8)-Lagrange pair for n.
 * https://oeis.org/A194520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12 a(1) = 3, a(2) =
 *            -2, a(3) = 1, a(4) = 4, a(5) = -1, a(6) = 2, a(7) = 5, a(8) = 0,
 *            a(9) = 3, a(10) = 4, a(11) = 1, a(12) = 4
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of terms computed so far */
    static size_t cache_cap = 0;     /* allocated capacity in elements */

    /* Ensure the cache can hold up to n terms (n is 1‑based) */
    if (cache_cap < n) {
        size_t new_cap = (n > 16) ? (size_t)(n * 2) : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first 12 values on first call */
    if (cache_len == 0) {
        static const seq_t init[12] = {
            3, -2, 1, 4, -1, 2,
            5, 0, 3, 4, 1, 4
        };
        for (idx_t i = 0; i < 12; ++i) {
            cache[i] = init[i];
        }
        cache_len = 12;
    }

    /* If requested term is already computed, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-11) - a(i-12) */
        cache[i - 1] = cache[i - 2]               /* a(i-1)  */
                     + cache[i - 12]             /* a(i-11) */
                     - cache[i - 13];            /* a(i-12) */
    }
    cache_len = n;

    return cache[n - 1];
}
