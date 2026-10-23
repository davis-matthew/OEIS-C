/*
 * A137470  Inverse binomial transform of 1, 2, 2, 4, 10, 20, ... = A100088.
 * https://oeis.org/A137470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   inverse binomial transform of 1,2,2,4,10,20,... Sequence
 *            satisfies: a(0)=1, a(1)=1, a(2)=-1; for n>=3: a(n) = -a(n-1) +
 *            2*a(n-3)
 * Range:     i8 n <= 14, i16 n <= 30, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;

    /* Initialize cache with first three terms.  Use explicit cast
       for compatibility with a big‑integer type in C++. */
    if (cache_sz == 0) {
        cache_sz = 3;
        cache = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-1;
    }

    /* Return already computed value. */
    if (n < cache_sz) {
        return cache[n];
    }

    /* Grow cache to hold terms up to n. */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_sz = (idx_t)(n + 1);

    /* Compute missing terms using the recurrence. */
    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = -a(i-1) + 2*a(i-3) */
        cache[i] = -cache[i-1] + (seq_t)2 * cache[i-3];
    }

    return cache[n];
}
