/*
 * A063371  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          98 ).
 * https://oeis.org/A063371
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n>13, initial values: a(2)
 *            = -1, a(3) = 95, a(4) = 188, a(5) = 282, a(6) = 376, a(7) = 470,
 *            a(8) = 564, a(9) = 659, a(10) = 752, a(11) = 847, a(12) = 940,
 *            a(13) = 1034
 * Range:     i8 n <= 3, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle indices below the offset; behavior is irrelevant. */
    if (n < 2) return 0;

    /* Static cache for values already computed. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* allocated size (elements, indexable 0..size-1) */
    static idx_t cache_last = 1;     /* last index initialized, so far 1 (no useful data) */

    /* Ensure storage covers index n. */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        /* Grow allocation. */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – give up */
        cache = tmp;
        cache_size = new_size;
    }

    /* Pre-fill initial values up to 13 on first use. */
    static const seq_t init_vals[14] = {
        0,                      /* 0 */
        0,                      /* 1 */
        -1,                     /* 2 */
        95,                     /* 3 */
        188,                    /* 4 */
        282,                    /* 5 */
        376,                    /* 6 */
        470,                    /* 7 */
        564,                    /* 8 */
        659,                    /* 9 */
        752,                    /* 10 */
        847,                    /* 11 */
        940,                    /* 12 */
        1034                    /* 13 */
    };

    /* Initialize the cache with the known initial values if needed. */
    if (cache_last < 13) {
        /* Ensure we have stored at least up to 13. */
        for (idx_t i = 2; i <= 13 && i < cache_size; ++i) {
            cache[i] = init_vals[i];
        }
        cache_last = 13;
    }

    /* Compute values up to requested n using the recurrence. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 13) {
            /* Use the supplied initial values. */
            cache[i] = init_vals[i];
        } else {
            cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
        }
    }
    cache_last = n;

    return cache[n];
}
