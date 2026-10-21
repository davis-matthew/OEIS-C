/*
 * A078046  Expansion of (1-x)/(1 + x + x^2 - x^3).
 * https://oeis.org/A078046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = -2, a(2) = 1; a(n) = a(n-3) - a(n-2) - a(n-1)
 *            for n > 2
 * Range:     i8 n <= 16, i16 n <= 33, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed terms */
    static seq_t *cache = NULL;     /* pointer to the array of seq_t values */
    static idx_t cache_len = 0;     /* number of elements currently stored (count) */

    /* ensure the cache can hold up to index n (inclusive) */
    if (n >= cache_len) {
        /* compute new size needed */
        idx_t new_len = n + 1;
        /* realloc the array to the new size */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure – return 0 (should not happen) */
        cache = new_cache;

        /* compute terms from the previous size up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = -((seq_t)2);
            } else if (i == 2) {
                cache[i] = (seq_t)1;
            } else {
                /* recurrence: a(i) = a(i-3) - a(i-2) - a(i-1) */
                cache[i] = cache[i-3] - cache[i-2] - cache[i-1];
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
