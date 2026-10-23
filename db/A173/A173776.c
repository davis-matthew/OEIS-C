/*
 * A173776  a(n) = (4*10^n + 41)/9.
 * https://oeis.org/A173776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*10^n + 41)/9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 5;  a(n) = 10*a(n-1) - 41 for n > 0 */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (cache_size <= n) {
        /* Grow or allocate cache */
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return zero (should not happen for test data) */
            return (seq_t)0;
        }
        cache = tmp;

        if (cache_size == 0) {
            /* First term */
            cache[0] = (seq_t)5;
            cache_size = 1;
        }
        /* Compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)10 - (seq_t)41;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
