/*
 * A041853  Denominators of continued fraction convergents to sqrt(448).
 * https://oeis.org/A041853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 254*a(n-2) - a(n-4) for n>3 a(0) = 1, a(1) = 6, a(2) =
 *            253, a(3) = 1524
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache      = NULL;      /* array of computed values   */
    static size_t cache_len = 0;          /* number of values stored    */
    static size_t cache_cap = 0;          /* allocated array capacity   */
    static const seq_t base[4] = { (seq_t)1,
                                   (seq_t)6,
                                   (seq_t)253,
                                   (seq_t)1524 };

    /* Lazy initial allocation of first four terms */
    if (cache == NULL) {
        cache_cap = 4;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 4; ++i) cache[i] = base[i];
        cache_len = 4;
    }

    /* Compute needed values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Ensure capacity for the new element */
        if (i == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {          /* allocation failure – give up */
                return (seq_t)0;
            }
            cache    = tmp;
            cache_cap = new_cap;
        }

        /* Recurrence or base initialization */
        if (i < 4) {
            cache[i] = base[i];
        } else {
            cache[i] = 254 * cache[i - 2] - cache[i - 4];
        }
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
