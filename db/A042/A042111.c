/*
 * A042111  Denominators of continued fraction convergents to sqrt(580).
 * https://oeis.org/A042111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 578*a(n-2) - a(n-4) for n > 3 Initial values: a(0)=1,
 *            a(1)=12, a(2)=577, a(3)=6936
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoised sequence values      */
    static size_t  cap   = 0;        /* current allocation capacity   */
    static idx_t   max_n = (idx_t)(-1); /* highest index that has been computed */

    /* Initialise the memoisation table with the first four terms. */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)577;
        cache[3] = (seq_t)6936;
        max_n  = 3;
    }

    /* If the requested term is already known, return it immediately. */
    if (n <= max_n) {
        return cache[n];
    }

    /* Grow the cache if necessary. */
    size_t new_cap = cap;
    while (new_cap <= (size_t)n) {
        new_cap *= 2;
    }
    if (new_cap > cap) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cap   = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i >= 4) {
            seq_t term = (seq_t)578 * cache[i - 2] - cache[i - 4];
            cache[i]   = term;
        }
    }
    max_n = n;
    return cache[n];
}
