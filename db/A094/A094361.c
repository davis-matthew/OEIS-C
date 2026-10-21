/*
 * A094361  Pair-reversal of 1, 4, 4, 16, 16, ... (A056450).
 * https://oeis.org/A094361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * a(n-2) with a(0) = 4, a(1) = 1 (pair‑reversal of 1,
 *            4, 4, 16, 16, …)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size   = 0;   /* number of elements stored   */
    static idx_t cache_limit  = 0;   /* allocated capacity          */

    /* ensure cache has space for at least n+1 elements */
    while (cache_size <= n) {
        /* initialise cache if it's the first call */
        if (cache_size == 0) {
            cache_limit = 2;
            cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)cache_limit);
            if (!cache) return 0;          /* out of memory – should not happen */
            cache[0] = 4;                  /* a(0) */
            cache[1] = 1;                  /* a(1) */
            cache_size = 2;
            continue;
        }

        /* grow the cache if necessary */
        if (cache_size >= cache_limit) {
            size_t new_limit = (cache_limit == 0) ? 2 : cache_limit * 2;
            seq_t *temp = (seq_t *)realloc(cache, sizeof(seq_t) * new_limit);
            if (!temp) return 0; /* out of memory – should not happen */
            cache = temp;
            cache_limit = new_limit;
        }

        /* compute a(cache_size) = 4 * a(cache_size-2) */
        cache[cache_size] = cache[cache_size - 2] * 4;
        ++cache_size;
    }

    return cache[n];
}
