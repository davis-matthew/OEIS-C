/*
 * A092806  Expansion of (1 + 8x - 9x^2)/(1 - 3x + 3x^2 - 1001x^3).
 * https://oeis.org/A092806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 11, a(2) = 21, a(n) = 3*a(n‑1) - 3*a(n‑2) +
 *            1001*a(n‑3) for n ≥ 3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of sequence A092806.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the first three elements */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) abort();   /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)21;
        cache_size = 3;
    }

    /* If the cache already holds the requested value, just return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* Grow the cache to accommodate up to index n */
    idx_t old_size = cache_size;
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cache_size = n + 1;
    }

    /* Compute the missing terms using the linear recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t term3 = (seq_t)1001 * cache[i - 3];
        seq_t term1 = (seq_t)3 * cache[i - 1];
        seq_t term2 = (seq_t)3 * cache[i - 2];
        cache[i] = term1 - term2 + term3;
    }

    return cache[n];
}
