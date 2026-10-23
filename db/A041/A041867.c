/*
 * A041867  Denominators of continued fraction convergents to sqrt(455).
 * https://oeis.org/A041867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 128*a(n-2) - a(n-4); a(0) = 1, a(1) = 3, a(2) = 127, a(3)
 *            = 384
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    if (n >= cache_cap) {
        idx_t old_cap = cache_cap;
        idx_t new_cap = n + 1;

        cache = (cache == NULL)
                ? (seq_t *)malloc(new_cap * sizeof(seq_t))
                : (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

        /* Compute new values */
        for (idx_t i = old_cap; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else if (i == 1) {
                cache[1] = 3;
            } else if (i == 2) {
                cache[2] = 127;
            } else if (i == 3) {
                cache[3] = 384;
            } else {
                /* recurrence: a(i) = 128*a(i-2) - a(i-4) */
                cache[i] = ((seq_t)cache[i - 2] << 7) - cache[i - 4];
            }
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
