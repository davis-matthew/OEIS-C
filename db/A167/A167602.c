/*
 * A167602  Expansion of 1/(1+14*x+72*x^2+384*x^3+512*x^4).
 * https://oeis.org/A167602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+4) + 14*a(n+3) + 72*a(n+2) + 384*a(n+1) + 512*a(n) = 0
 *            Initial values: a(0)=1, a(1)=-14, a(2)=124, a(3)=-1112
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 18, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms (1‑based indices) */
    static seq_t *cache     = NULL;    /* array of computed a(i) */
    static idx_t cache_cap  = 0;       /* allocated capacity in elements */
    static idx_t cache_len  = 0;       /* number of values that are already set */

    /* Ensure enough space for the requested index */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* First time: initialise the first four terms of the recurrence */
    if (cache_len == 0) {
        cache[0] = (seq_t)  1;
        cache[1] = (seq_t) -14;
        cache[2] = (seq_t) 124;
        cache[3] = (seq_t)-1112;
        cache_len = 4;                /* we now have values 0..3 defined */
    }

    /* If the requested index is already computed, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* Compute any missing terms up to index n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = -(
            ((seq_t)14   * cache[i-1]) +
            ((seq_t)72   * cache[i-2]) +
            ((seq_t)384  * cache[i-3]) +
            ((seq_t)512  * cache[i-4]));
        cache[i] = val;
    }

    /* Update the length of cached values */
    if (n + 1 > cache_len) {
        cache_len = n + 1;
    }

    return cache[n];
}
