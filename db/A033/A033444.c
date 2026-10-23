/*
 * A033444  Number of edges in 12-partite Turán graph of order n.
 * https://oeis.org/A033444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} floor(11*i/12)
 * Range:     u8 n <= 23, u16 n <= 378, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of a(0..max_n).  cache[0] = 0.  */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of terms cached (>=1) */

    /* Ensure that cache[0] is initialized. */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* fallback – unlikely to happen */
        cache[0] = (seq_t)0;
    }

    /* Grow the cache if necessary. */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* fallback – unlikely to happen */
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            /* Compute the incremental addition: floor(11*i/12) */
            idx_t inc = (i * (idx_t)11) / 12;
            cache[i] = cache[i - 1] + (seq_t)inc;
        }
        cache_len = new_len;
    }

    return cache[n];
}
