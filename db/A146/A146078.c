/*
 * A146078  Expansion of 1/(1-x*(1-9*x)).
 * https://oeis.org/A146078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 9*a(n-2), a(0)=1, a(1)=1
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 19, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoization cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* First-time initialization for cache */
    if (!cache) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* In case of allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* Return from cache if already computed */
    if (n < cache_len) return cache[n];

    /* Extend cache up to the requested index */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* Allocation failure */
    cache = tmp;

    /* Compute remaining terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i - 1] - (seq_t)9 * cache[i - 2];
    }
    cache_len = new_len;

    return cache[n];
}
