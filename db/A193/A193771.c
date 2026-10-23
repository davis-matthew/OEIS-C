/*
 * A193771  Expansion of 1 / (1 - x - x^3 + x^6) in powers of x.
 * https://oeis.org/A193771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-6); Initial values: a(0) = 1, a(1)
 *            = 1, a(2) = 1, a(3) = 2, a(4) = 3, a(5) = 4. (a(n) = 0 for n <
 *            0)
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 78, u64 n <= 157,
 *            u128 n <= 315, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;   /* array of values */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t len   = 0;      /* number of values already stored */

    /* If n is already computed, simply return it. */
    if (n < (idx_t)len)
        return cache[(size_t)n];

    /* Determine new capacity: next power of two ≥ n+1. */
    size_t new_cap = (cap ? cap : 1);
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    /* Reallocate cache to the new capacity.  Cast is needed for C++11. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    cache = tmp;
    cap   = new_cap;

    /* Initialise the first six terms if this is the first allocation. */
    if (len == 0) {
        /* Ensure capacity covers the base indices 0..5. */
        if (cap < 6) {
            size_t tmp_cap = 6;
            seq_t *tmp2 = (seq_t *)realloc(cache, tmp_cap * sizeof(seq_t));
            if (!tmp2) abort();
            cache = tmp2;
            cap   = tmp_cap;
        }
        cache[0] = 1;  /* a(0) */
        cache[1] = 1;  /* a(1) */
        cache[2] = 1;  /* a(2) */
        cache[3] = 2;  /* a(3) */
        cache[4] = 3;  /* a(4) */
        cache[5] = 4;  /* a(5) */
        len = 6;       /* indices 0 through 5 are now valid */
    }

    /* Compute any missing values up to n using the recurrence. */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i >= 6) {
            cache[i] = cache[i - 1] + cache[i - 3] - cache[i - 6];
        }
        /* i < 6 cases are already initialised. */
    }

    /* Update the number of stored values. */
    if (len < n + 1)
        len = n + 1;

    return cache[(size_t)n];
}
