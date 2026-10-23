/*
 * A042746  Numerators of continued fraction convergents to sqrt(904).
 * https://oeis.org/A042746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 902*a(n-2) - a(n-4) (n ≥ 4) Initial values: a(0)=30,
 *            a(1)=451, a(2)=27090, a(3)=406801
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
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
    static size_t len = 0;               /* number of terms currently stored */

    /* n may be any unsigned integer, so cast to size_t for array indices */
    size_t idx = (size_t)n;

    /* Initialise the cache with the first four terms */
    if (len == 0) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)30;
        cache[1] = (seq_t)451;
        cache[2] = (seq_t)27090;
        cache[3] = (seq_t)406801;
        len = 4;
    }

    /* Grow the cache if the requested index is beyond the current capacity */
    if (idx >= cap) {
        size_t new_cap = cap ? (cap << 1) : 4;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* If we already have the requested term, return it immediately */
    if (idx < len)
        return cache[idx];

    /* Compute all missing terms up to idx */
    const seq_t mult = (seq_t)902;
    for (size_t i = len; i <= idx; ++i) {
        seq_t val;
        if (i < 4) { /* safety guard – should not normally hit */
            if (i == 0) val = (seq_t)30;
            else if (i == 1) val = (seq_t)451;
            else if (i == 2) val = (seq_t)27090;
            else           val = (seq_t)406801;
        } else {
            /* a(i) = 902 * a(i-2) - a(i-4) */
            val = mult * cache[i - 2] - cache[i - 4];
        }
        cache[i] = val;
    }
    len = idx + 1;
    return cache[idx];
}
