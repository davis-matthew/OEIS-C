/*
 * A099493  Expansion of (1+x^2)^2/(1+x^2-2x^3+x^4+x^6).
 * https://oeis.org/A099493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-2) + 2*a(n-3) - a(n-4) - a(n-6), with initial values
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=2, a(4)=-1, a(5)=0, offset 0
 * Range:     i8 n <= 19, i16 n <= 35, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the OEIS sequence A099493. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache of already computed terms   */
    static idx_t cache_size = 0;         /* current size of the cache        */

    /* initialise cache with the first six terms if needed */
    if (cache == NULL) {
        cache_size = 6;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* fallback on allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)(-1);
        cache[5] = (seq_t)0;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* extend the cache to hold the requested index */
    size_t old_size = cache_size;
    cache_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
    if (tmp == NULL) {
        /* if reallocation fails, keep the old cache and return zero as fallback */
        return (seq_t)0;
    }
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        /* recurrence requires i >= 6, which is guaranteed here
           because old_size >= 6 and we only compute for i >= old_size  */
        seq_t term = -(cache[i-2]) + (seq_t)2 * cache[i-3] - cache[i-4] - cache[i-6];
        cache[i] = term;
    }

    return cache[n];
}
