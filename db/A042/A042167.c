/*
 * A042167  Denominators of continued fraction convergents to sqrt(608).
 * https://oeis.org/A042167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of sqrt(608) continued
 *            fraction
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    /* Periodic part of the continued fraction of sqrt(608):
       [1, 1, 1, 11, 1, 1, 1, 48]     (length 8) */
    static const unsigned period[8] = {1, 1, 1, 11, 1, 1, 1, 48};
    static const size_t period_len = 8;

    if (n >= cache_size) {
        /* Grow the cache if necessary */
        size_t new_cap = cache_cap ? cache_cap : 8;
        if (new_cap <= (size_t)n) {
            new_cap = (size_t)n + 1;
            if (new_cap < 8) new_cap = 8;
        }
        while (new_cap <= (size_t)n) new_cap <<= 1;

        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;

        /* Compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t q;
            if (i == 0) {
                q = (seq_t)1;                     /* q(0) = 1 */
            } else if (i == 1) {
                unsigned a = period[0];
                q = (seq_t)(a * cache[0]);       /* q(1) = a1 * q(0) */
            } else {
                unsigned a = period[(i - 1) % period_len];
                q = (seq_t)(a * cache[i - 1] + cache[i - 2]); /* recurrence */
            }
            cache[i] = q;
        }
        cache_size = (size_t)n + 1;
    }

    return cache[n];
}
