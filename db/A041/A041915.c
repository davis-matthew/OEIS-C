/*
 * A041915  Denominators of continued fraction convergents to sqrt(479).
 * https://oeis.org/A041915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1, a(1) = 1, a(n) = a(n‑12) * 5978880 – a(n‑24) for n>23
 *            (recurrence for denominators of convergents to sqrt(479))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence term type and index type are supplied by the build. */
seq_t a(idx_t n)
{
    /* coefficients of the continued fraction for sqrt(479) after a0.
       They repeat with period 12: [1,7,1,3,2,21,2,3,1,7,1,42] */
    static const seq_t cf[12] = {
        (seq_t)1, (seq_t)7, (seq_t)1, (seq_t)3,
        (seq_t)2, (seq_t)21, (seq_t)2, (seq_t)3,
        (seq_t)1, (seq_t)7, (seq_t)1, (seq_t)42
    };

    /* Cache of already computed terms. */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static size_t filled  = 0;

    /* Ensure the cache can hold at least n+1 terms. */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = cap ? cap<<1 : 32;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* If we have not initialised base terms yet, do it now. */
    if (filled == 0) {
        cache[0] = (seq_t)1;
        if (cap > 1) cache[1] = (seq_t)1;
        filled = 2;
    }

    /* Compute missing terms up to n. */
    for (size_t i = filled; i <= (size_t)n; ++i) {
        if (i < 2) { /* base terms already set */
            continue;
        }
        /* coefficient index for a_i: cf[(i-1) % 12] */
        size_t ci = (i - 1) % 12;
        cache[i] = cf[ci] * cache[i-1] + cache[i-2];
    }

    filled = needed;
    return cache[(size_t)n];
}
