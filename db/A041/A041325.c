/*
 * A041325  Denominators of continued fraction convergents to sqrt(176).
 * https://oeis.org/A041325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 398*a(n-4) - a(n-8) (for n >= 8) Initial values: a(0) =
 *            1, a(1) = 3, a(2) = 4, a(3) = 15, a(4) = 394, a(5) = 1197, a(6)
 *            = 1591, a(7) = 5970
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init_vals[8] = {1, 3, 4, 15, 394, 1197, 1591, 5970};

seq_t a(idx_t n) {
    /* Handle the first eight known terms directly. */
    if (n < 8)
        return _init_vals[n];

    /* Static cache for dynamic computation and memoisation. */
    static seq_t *cache = NULL;    /* allocated array of sequence values */
    static idx_t cache_cap   = 0;  /* number of elements currently allocated */
    static idx_t cache_last  = 0;  /* index of last computed value */

    /* Initialise cache on first use with the first eight values. */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t *)calloc((size_t)cache_cap, sizeof(seq_t));
        if (!cache)
            exit(1);               /* allocation failure */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = _init_vals[i];
        cache_last = 7;
    }

    /* Grow cache if a larger n is requested. */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);               /* allocation failure */
        /* Initialise the new slots with zero (placeholder). */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    if (n > cache_last) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            cache[i] = 398 * cache[i - 4] - cache[i - 8];
        }
        cache_last = n;
    }

    return cache[n];
}
