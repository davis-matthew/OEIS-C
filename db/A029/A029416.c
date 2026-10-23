/*
 * A029416  Expansion of 1/((1-x^6)*(1-x^7)*(1-x^8)*(1-x^12)).
 * https://oeis.org/A029416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029416: Number of partitions of n into parts 6, 7, 8, and 12. */
/* Computed by dynamic programming with unlimited coin change. */
/* a(0)=1, a(n)=0 for 1≤n<6, and for n≥0 a(n)=∑_{p∈{6,7,8,12}} a(n−p)  for n≥p. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values up to the largest n seen so far. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;     /* number of allocated slots (>= current max + 1) */

    /* Resize cache if necessary and recompute all coefficients up to n. */
    if ((idx_t)cache_size <= n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure – return 0 (safe fallback) */
        cache = tmp;
        cache_size = new_size;

        /* Initialise to 0, except cache[0]=1. */
        for (size_t i = 1; i <= (size_t)n; ++i) cache[i] = 0;
        cache[0] = 1;

        /* Parts allowed in the partitions. */
        const idx_t parts[4] = {6, 7, 8, 12};

        /* Dynamic programming: for each part, update all amounts ≥ part. */
        for (int k = 0; k < 4; ++k) {
            idx_t part = parts[k];
            for (idx_t i = part; i <= n; ++i) {
                cache[i] += cache[i - part];
            }
        }
    }

    /* cache[n] now holds the desired value. */
    return cache[n];
}
