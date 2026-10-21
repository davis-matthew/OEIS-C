/*
 * A097836  Chebyshev polynomials S(n,51).
 * https://oeis.org/A097836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 51*a(n-1) - a(n-2); a(0)=1, a(1)=51
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;

    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)51;

    /* Initialise cache if it has not been used yet */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;                /* allocation failed – return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)51;
    }

    /* If the requested index is already in the cache, return it */
    if (n < cache_size) return cache[n];

    /* Expand the cache up to index n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
    if (!tmp) return (seq_t)0;                        /* allocation failed – return 0 */
    cache = tmp;

    /* Compute the new terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = (seq_t)51 * cache[i - 1] - cache[i - 2];
    }
    cache_size = new_size;

    return cache[n];
}
