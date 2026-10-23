/*
 * A163868  a(n) = (4^n + 11)/3.
 * https://oeis.org/A163868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n + 11)/3 Equivalent recurrence: a(0) = 4, a(n) =
 *            4*a(n-1) - 11 (n > 0)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to memoize already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t last_cached = 0;

    /* initialize cache with a(0) */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) abort();          /* memory allocation failed */
        cache[0] = (seq_t)4;                 /* a(0) = 4 */
        last_cached = 0;
    }

    /* extend cache if the requested index is beyond current size */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();           /* memory allocation failed */
        cache = tmp;
        cache_size = new_size;

        /* compute terms from last_cached+1 up to n using the recurrence */
        for (idx_t i = last_cached + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)4 - (seq_t)11;
        }
        last_cached = n;
    }

    return cache[n];
}
