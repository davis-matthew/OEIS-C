/*
 * A099858  A Chebyshev transform of (1+3x)/(1-3x).
 * https://oeis.org/A099858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 3*a(n-1) - 2*a(n-2) + 3*a(n-3) - a(n-4)
 * with initial values
 *     a(0) = 1
 *     a(1) = 6
 *     a(2) = 17
 *     a(3) = 42
 */

#include <stdlib.h>   /* for realloc */

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;   /* number of entries currently stored */

    /* If we already have the requested value, return it immediately */
    if (n < cache_size)
        return cache[n];

    /* Ensure the cache holds at least the first 4 terms (needed for the recurrence) */
    if (cache_size < 4) {
        idx_t new_size = 4;
        if (new_size < n + 1)
            new_size = n + 1;        /* we may need more entries for the current call */

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failed */

        cache = tmp;
        cache_size = new_size;

        /* Initialise the first four terms */
        cache[0] = (seq_t)1;
        if (cache_size > 1) cache[1] = (seq_t)6;
        if (cache_size > 2) cache[2] = (seq_t)17;
        if (cache_size > 3) cache[3] = (seq_t)42;

        /* If the caller requested a value below 4, we are done */
        if (n < 4)
            return cache[n];
    }

    /* Extend the cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();

        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing values using the recurrence */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t term = 3 * cache[i-1] + 3 * cache[i-3];
        term = term - 2 * cache[i-2];
        term = term - cache[i-4];
        cache[i] = term;
    }

    return cache[n];
}
