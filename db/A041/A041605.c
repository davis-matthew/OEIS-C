/*
 * A041605  Denominators of continued fraction convergents to sqrt(320).
 * https://oeis.org/A041605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 322*a(n-4) - a(n-8) for n > 7 Initial values: a(0) = 1,
 *            a(1) = 1, a(2) = 8, a(3) = 9, a(4) = 314, a(5) = 323, a(6) =
 *            2575, a(7) = 2898
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static const seq_t init[8] = {1, 1, 8, 9, 314, 323, 2575, 2898};

    /* initialise cache with the first 8 terms */
    if (cache == NULL) {
        cap = 8;
        /* malloc returns void* which must be cast in C++ */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure */
        for (idx_t i = 0; i < 8; ++i) cache[i] = init[i];
    }

    /* if the requested index is already computed, return it */
    if (n < cap) return cache[n];

    /* expand cache to accommodate up to index n */
    size_t newcap = (size_t)n + 1;
    if (newcap > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure */
        cache = tmp;
        /* fill missing values using the recurrence */
        for (idx_t i = (idx_t)cap; i <= n; ++i) {
            cache[i] = 322 * cache[i-4] - cache[i-8];
        }
        cap = newcap;
    } else {
        /* newcap <= cap: all needed values are already computed */
    }

    return cache[n];
}
