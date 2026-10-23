/*
 * A029548  Expansion of 1/(1 - 32*x + x^2).
 * https://oeis.org/A029548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*a(n-1) - a(n-2), a(0) = 1, a(1) = 32, n >= 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store already computed terms */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0xFFFFFFFFFFFFFFFFULL; /* sentinel: no cache yet */

    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)32;

    /* Initialize cache on first call */
    if (cache == NULL) {
        /* Allocate space for at least two terms */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure, should not happen */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)32;
        max_cached = 1;
    }

    /* If requested term is beyond current cache, extend it */
    if (n > max_cached) {
        idx_t new_size = n + 1;                    /* new array length */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;         /* allocation failure */
        cache = tmp;

        /* Compute new terms using the recurrence */
        for (idx_t k = max_cached + 1; k <= n; ++k) {
            cache[k] = cache[k - 1] * (seq_t)32 - cache[k - 2];
        }
        max_cached = n;
    }

    return cache[n];
}
