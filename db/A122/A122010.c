/*
 * A122010  G.f. x^2*(1-5*x)/(1-12*x+15*x^2-2*x^3).
 * https://oeis.org/A122010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 15*a(n-2) + 2*a(n-3) Initial values: a(1)=0,
 *            a(2)=1, a(3)=7
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms (1‑indexed) */
    static seq_t *cache = NULL;
    static idx_t  len   = 0;   /* number of terms stored so far */
    static idx_t  cap   = 0;   /* allocated capacity */

    /* Ensure the cache contains the first three terms. */
    if (len < 3 && len < n) {
        size_t new_cap = cap < 4 ? 4 : cap;
        while (new_cap < 3) new_cap <<= 1;
        if (new_cap > cap) {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cap   = new_cap;
        }
        if (len == 0) {
            cache[0] = 0; /* a(1) */
            cache[1] = 1; /* a(2) */
            cache[2] = 7; /* a(3) */
            len = 3;
        }
    }

    /* If more terms are required, grow the cache and compute them. */
    if (n > len) {
        /* Ensure enough capacity. */
        if ((idx_t)cap < n) {
            size_t new_cap = cap < 4 ? 4 : cap;
            while (new_cap < n) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cap   = new_cap;
        }

        /* Compute missing terms using the recurrence. */
        for (idx_t i = len + 1; i <= n; ++i) {
            seq_t val = 12 * cache[i-2] - 15 * cache[i-3] + 2 * cache[i-4];
            cache[i-1] = val;
        }
        len = n;
    }

    return cache[n-1];
}
