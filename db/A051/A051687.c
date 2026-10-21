/*
 * A051687  a(n) = (5*n+6)(!^5)/6, related to A008548 ((5*n+1)(!^5) quintic,
 *          or 5-factorials).
 * https://oeis.org/A051687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k=1}^n (5k+6), a(0)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache[0] = 1 */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)1;
    }

    /* if the value is already cached, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* enlarge cache up to n */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;  /* allocation failure – unlikely in tests */
    cache = new_cache;

    /* compute missing values */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t factor = (seq_t)(5 * (seq_t)i + 6);
        cache[i] = cache[i - 1] * factor;
    }

    cache_len = new_len;
    return cache[n];
}
