/*
 * A180029  Eight white queens and one red queen on a 3 X 3 chessboard. G.f.:
 *          (1 + 2*x)/(1 - 6*x - 2*x^2).
 * https://oeis.org/A180029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) + 2*a(n-2) with a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to memoize computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;             /* allocated size */
    static idx_t  max_computed = (idx_t)(-1); /* largest n computed */

    /* initialize cache on first call */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out of memory, unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        max_computed = 1;
    }

    /* if requested n already computed, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* ensure capacity to store upto n */
    if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* compute values from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)6 + cache[i - 2] * (seq_t)2;
    }
    max_computed = n;
    return cache[n];
}
