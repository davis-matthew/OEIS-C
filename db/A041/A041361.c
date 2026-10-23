/*
 * A041361  Denominators of continued fraction convergents to sqrt(194).
 * https://oeis.org/A041361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 390*a(n-4) - a(n-8) for n >= 8 Initial values: a(0) = 1,
 *            a(1) = 1, a(2) = 13, a(3) = 14, a(4) = 377, a(5) = 391, a(6) =
 *            5069, a(7) = 5460
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;   /* array holding a(i) */
    static size_t cap = 0;        /* allocated size */
    static idx_t last = 0;        /* highest index that has been computed */
    const seq_t init[8] = {1,1,13,14,377,391,5069,5460};

    /* Initialise cache the first time a() is called */
    if (!cache) {
        size_t need = 8;
        size_t new_cap = 1;
        while (new_cap < need) new_cap <<= 1;
        cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        cap = new_cap;
        for (idx_t i = 0; i < 8; ++i) cache[i] = init[i];
        last = 7;
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i >= 8) {
                /* recurrence: a(i) = 390*a(i-4) - a(i-8) */
                cache[i] = cache[i-4] * (seq_t)390 - cache[i-8];
            } else {
                /* already initialised */
            }
        }
        last = n;
    }

    return cache[n];
}
