/*
 * A028076  Expansion of 1/((1-3x)(1-6x)(1-7x)(1-10x)).
 * https://oeis.org/A028076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (for n ≥ 4):  a(n) = 26*a(n-1) - 241*a(n-2) + 936*a(n-3) - 1260*a(n-4)
   initial values: a(0) = 1, a(1) = 26, a(2) = 435, a(3) = 5980 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache holds the computed terms for all n up to current cache_len-1 */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;

    /* Initialise cache with the first four known values */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;   /* allocation failure (unlikely) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)435;
        cache[3] = (seq_t)5980;
    }

    /* If the requested term is already in cache, return it */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to include up to index n */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
    if (!tmp) return (seq_t)0;          /* allocation failure (unlikely) */
    cache = tmp;

    /* Compute missing terms using the recurrence relation */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)26 * cache[i-1]
                 - (seq_t)241 * cache[i-2]
                 + (seq_t)936 * cache[i-3]
                 - (seq_t)1260 * cache[i-4];
    }

    cache_len = new_len;
    return cache[n];
}
