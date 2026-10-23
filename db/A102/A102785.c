/*
 * A102785  G.f.: (x-1)/(-2*x^2 + 3*x^3 + 2*x - 1).
 * https://oeis.org/A102785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+3) = 2*a(n+2) - 2*a(n+1) + 3*a(n) a(0) = 1, a(1) = 1, a(2) =
 *            0
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with first three terms on the first call */
    if (cache_size == 0) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
    }

    /* if requested index already computed, simply return it */
    if (n < cache_size)
        return cache[n];

    /* extend cache up to index n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();              /* allocation failure */
    cache = tmp;

    /* fill missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* a(i) = 2*a(i-1) - 2*a(i-2) + 3*a(i-3) */
        seq_t val = 2 * cache[i - 1];
        val += 3 * cache[i - 3];
        val -= 2 * cache[i - 2];
        cache[i] = val;
    }

    cache_size = new_size;
    return cache[n];
}
