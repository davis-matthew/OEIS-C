/*
 * A199930  Trisection 0 of A199803.
 * https://oeis.org/A199930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = -2, a(2) = -1, a(3) = 12 a(n) = -a(n-1) -
 *            5*a(n-2) + a(n-3) - a(n-4) for n > 3
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 26, i64 n <= 53, i128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for already computed values.  */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure that cache holds values up to index n (inclusive).  */
static void ensure_cache_size(idx_t n)
{
    if (n < cache_size)
        return;                                 /* already available */

    /* Grow cache to at least n+1 entries.  */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();                     /* out of memory */

    cache = new_cache;

    /* Initialise new entries.  */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)
            cache[0] = 1;
        else if (i == 1)
            cache[1] = (seq_t)-2;
        else if (i == 2)
            cache[2] = (seq_t)-1;
        else if (i == 3)
            cache[3] = 12;
        else
            cache[i] = -cache[i-1] - 5*cache[i-2] + cache[i-3] - cache[i-4];
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* Create initial cache if this is the first call.  */
    if (!cache) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) abort();                     /* out of memory */
        cache[0] = 1;
        cache[1] = (seq_t)-2;
        cache[2] = (seq_t)-1;
        cache[3] = 12;
        cache_size = 4;
    }

    ensure_cache_size(n);
    return cache[n];
}
