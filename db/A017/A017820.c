/*
 * A017820  Expansion of 1/(1-x^3-x^4-x^5-x^6-x^7).
 * https://oeis.org/A017820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7) (n > 6) Initial
 *            values: a(0) = 1, a(1) = 0, a(2) = 0, a(3) = 1, a(4) = 1, a(5) =
 *            1, a(6) = 2 (Number of compositions of n into parts 3,4,5,6,7)
 * Range:     u8 n <= 20, u16 n <= 36, u32 n <= 68, u64 n <= 132,
 *            u128 n <= 261, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of stored values (highest index + 1) */

    /* initial cache setup */
    if (cache == NULL) {
        cache_len = 7;                         /* store indices 0..6 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)1;
        cache[6] = (seq_t)2;
    }

    /* if n is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* grow cache to hold up to n */
    size_t new_len = (size_t)n + 1;
    if (new_len > (size_t)cache_len) {
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* recompute values from old length up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 6) {
                /* base values are already set; skip */
                continue;
            }
            seq_t sum = cache[i - 3] + cache[i - 4]
                      + cache[i - 5] + cache[i - 6]
                      + cache[i - 7];
            cache[i] = sum;
        }
        cache_len = new_len;
    }

    return cache[n];
}
