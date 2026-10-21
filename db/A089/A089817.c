/*
 * A089817  a(n) = 5*a(n-1) - a(n-2) + 1 with a(0)=1, a(1)=6.
 * https://oeis.org/A089817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - a(n-2) + 1 (a(0)=1, a(1)=6)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;         /* number of elements stored in cache */

    /* initialise cache for the first two terms */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* failure – should not happen */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
    }

    /* extend cache if needed */
    if (n >= cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;   /* failure – should not happen */
        cache = new_cache;
        for (idx_t k = cache_len; k <= n; ++k) {
            /* recurrence: a(k) = 5*a(k-1) - a(k-2) + 1 */
            seq_t t = (seq_t)5 * cache[k-1] - cache[k-2] + (seq_t)1;
            cache[k] = t;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
