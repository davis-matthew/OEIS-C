/*
 * A162273  a(n) = ((2+sqrt(3))*(3+sqrt(3))^n + (2-sqrt(3))*(3-sqrt(3))^n)/2.
 * https://oeis.org/A162273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 6*a(n-2), a(0) = 2, a(1) = 9 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements currently initialized */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache with first two terms if empty */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 2;
        cache[1] = 9;
        cache_len = 2;
    }

    /* grow cache if needed */
    if (n >= cache_len) {
        if ((idx_t)(cache_cap) <= n) {
            size_t new_cap = cache_cap;
            while (new_cap <= (size_t)n) new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        /* compute missing terms up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t diff = cache[i - 1] - cache[i - 2];
            cache[i] = diff * (seq_t)6;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
