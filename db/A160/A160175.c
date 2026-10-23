/*
 * A160175  Expansion of 1/(1 - 2*x - 2*x^2 - 2*x^3 - 2*x^4).
 * https://oeis.org/A160175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(a(n-1)+a(n-2)+a(n-3)+a(n-4)); a(0)=1, a(1)=2, a(2)=6,
 *            a(3)=18
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
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

    /* initialise cache with the first four terms */
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;               /* allocation failed (unlikely) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)18;
    }

    /* value already computed? */
    if (n < cache_len)
        return cache[n];

    /* grow cache to hold up to index n */
    size_t new_len = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!cache) return (seq_t)0;               /* allocation failed (unlikely) */

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4];
        cache[i] = sum + sum; /* 2 * sum */
    }
    cache_len = (idx_t)new_len;
    return cache[n];
}
