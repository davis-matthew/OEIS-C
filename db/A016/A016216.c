/*
 * A016216  Expansion of 1 / ((1-x) * (1-3*x) * (1-11*x)).
 * https://oeis.org/A016216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 15, a(n) = 14*a(n-1) - 33*a(n-2) + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache     = NULL;  /* array of computed terms */
    static idx_t  cache_len = 0;     /* number of elements in cache */

    /* Initialise the first two terms if not done yet */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(1);          /* fatal error on allocation failure */
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)15;         /* a(1) */
    }

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Extend cache up to index n */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) exit(1);          /* fatal error on allocation failure */
    cache = new_cache;

    /* Compute new terms using the recurrence */
    for (idx_t i = cache_len; i < new_len; ++i) {
        seq_t term = cache[i-1] * (seq_t)14;
        term = term - cache[i-2] * (seq_t)33;
        term = term + (seq_t)1;
        cache[i] = term;
    }

    cache_len = new_len;
    return cache[n];
}
