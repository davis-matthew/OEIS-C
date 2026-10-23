/*
 * A166122  a(n) = (7-(-5)^n)/6.
 * https://oeis.org/A166122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 14, i64 n <= 28, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n) = 7 - 5 * a(n-1) for n > 0 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow repeated calls in any order */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of elements stored (>=1 when cache != NULL) */

    if (n >= cache_size) {
        /* Allocate or grow the cache to hold indices 0 .. n */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(*cache));
        if (!new_cache) {
            /* allocation failure: return 0 as fallback */
            return (seq_t)0;
        }
        cache = new_cache;

        /* Initialize new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else {
                cache[i] = (seq_t)7 - (seq_t)5 * cache[i - 1];
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
