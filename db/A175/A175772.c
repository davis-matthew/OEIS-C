/*
 * A175772  Expansion of 1/(1 - x - x^9 - x^17 + x^18).
 * https://oeis.org/A175772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-9) + a(n-17) - a(n-18) Initial values (OEIS
 *            offset 0): a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=1,
 *            a(6)=1, a(7)=1, a(8)=1, a(9)=2, a(10)=3, a(11)=4, a(12)=5,
 *            a(13)=6, a(14)=7, a(15)=8, a(16)=9, a(17)=11
 * Range:     u8 n <= 32, u16 n <= 61, u32 n <= 117, u64 n <= 231,
 *            u128 n <= 457, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* first unused index in cache */
    static idx_t cache_size = 0;  /* allocated size */

    /* initial 18 values */
    static const seq_t init[18] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        3, 4, 5, 6, 7, 8, 9, 11
    };

    /* if requested value already cached, just return it */
    if (n < cache_len)
        return cache[n];

    /* grow cache to accommodate n */
    if (cache == NULL || (n + 1) > cache_size) {
        idx_t new_size = (cache_size != 0 ? cache_size << 1 : (idx_t)64);
        while (new_size <= n)
            new_size <<= 1U;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_size);
        if (tmp == NULL) {
            /* allocation failure: abort (could also handle error) */
            abort();
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 18) {
            cache[i] = init[i];
        } else {
            /* recurrence: a(i) = a(i-1) + a(i-9) + a(i-17) - a(i-18) */
            cache[i] = cache[i-1] + cache[i-9] + cache[i-17] - cache[i-18];
        }
    }
    cache_len = n + 1;

    return cache[n];
}
