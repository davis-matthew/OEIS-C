/*
 * A041082  Numerators of continued fraction convergents to sqrt(48).
 * https://oeis.org/A041082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 14*a(n-2) - a(n-4) Initial values: a(0) = 6, a(1) = 7,
 *            a(2) = 90, a(3) = 97
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized recurrence for the sequence A041082 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached values */
    static size_t cache_len = 0;     /* number of values stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* Ensure cache can hold indices up to n */
    if (n >= cache_len) {
        size_t required = (size_t)n + 1;
        if (required > cache_cap) {
            size_t new_cap = cache_cap == 0 ? 8 : cache_cap;
            while (new_cap < required)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* memory allocation failed; abort */
                abort();
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialize base terms if not yet filled */
        if (cache_len == 0) {
            cache[0] = (seq_t)6;
            cache_len = 1;
        }
        if (cache_len == 1) {
            cache[1] = (seq_t)7;
            cache_len = 2;
        }
        if (cache_len == 2) {
            cache[2] = (seq_t)90;
            cache_len = 3;
        }
        if (cache_len == 3) {
            cache[3] = (seq_t)97;
            cache_len = 4;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)14 * cache[i - 2] - cache[i - 4];
        }
        cache_len = required;
    }

    return cache[n];
}
