/*
 * A142722  a(0)=a(1)=1 and a(n) = -(2*n-1)*a(n-1) + a(n-2) for n>=2.
 * https://oeis.org/A142722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 1
   a(n) = -(2*n-1)*a(n-1) + a(n-2)   for n >= 2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* If the first two terms are requested, return immediately. */
    if (n < 2)
        return (seq_t)1;

    /* Static cache to store already computed values. */
    static seq_t *cache = NULL;
    static size_t   cache_cap = 0;
    size_t prior_cap = cache_cap;

    /* Allocate cache if this is the first call. */
    if (!cache) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)1;           /* allocation failed – best‑effort return */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }
    /* Grow cache if necessary. */
    else if (cache_cap <= (size_t)n) {
        size_t need = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp)
            return (seq_t)1;
        cache = tmp;
        cache_cap = need;
    }

    /* Compute missing terms up to n. */
    size_t start = prior_cap;
    if (start < 2)
        start = 2;
    for (size_t i = start; i <= (size_t)n; ++i) {
        cache[i] = - ( (seq_t)(2 * i - 1) * cache[i-1] ) + cache[i-2];
    }

    return cache[n];
}
