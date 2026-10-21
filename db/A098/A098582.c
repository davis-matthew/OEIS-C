/*
 * A098582  Expansion of (1+2*x+4*x^2+8*x^3)/(1-x-16*x^5).
 * https://oeis.org/A098582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098582
   Formula: a(n) = a(n-1) + 16*a(n-5),  n >= 5
   Initial terms: a(0)=1, a(1)=3, a(2)=7, a(3)=15, a(4)=15            */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;

    /* initialise the first five terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)15;
        cached_len = 5;
    }

    /* return from cache if already computed */
    if (n < cached_len)
        return cache[n];

    /* extend the cache up to index n */
    idx_t new_len = n + 1;
    if (new_len > cached_len) {
        /* reallocate larger array */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;   /* out of memory: return 0 as safe fallback */
        cache = tmp;
        /* compute additionally demanded terms */
        for (idx_t i = cached_len; i < new_len; ++i) {
            cache[i] = cache[i - 1] + (seq_t)16 * cache[i - 5];
        }
        cached_len = new_len;
    }
    return cache[n];
}
