/*
 * A217995  Number of n X 1 arrays of the minimum or maximum value of
 *          corresponding elements and their horizontal and vertical neighbors
 *          in a random 0..2 n X 1 array.
 * https://oeis.org/A217995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 2*a(n-3) - 13*a(n-4) - 7*a(n-5) +
 *            3*a(n-6), a(1)=3, a(2)=9, a(3)=25, a(4)=81, a(5)=241, a(6)=719
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to store already computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0)        /* undefined, but return 0 for safety */
        return 0;

    /* Grow cache if necessary. */
    if (cache_len < n) {
        size_t new_len = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {  /* allocation failed */
            abort();      /* terminate program */
        }
        cache = new_cache;

        /* Compute missing terms up to n. */
        for (idx_t i = cache_len; i < n; ++i) {
            if (i == 0)
                cache[i] = 3;
            else if (i == 1)
                cache[i] = 9;
            else if (i == 2)
                cache[i] = 25;
            else if (i == 3)
                cache[i] = 81;
            else if (i == 4)
                cache[i] = 241;
            else if (i == 5)
                cache[i] = 719;
            else {
                /* Recurrence: a(n) = 3*a(n-1)+a(n-2)+2*a(n-3)-13*a(n-4)-7*a(n-5)+3*a(n-6) */
                seq_t pos = (seq_t)3 * cache[i-1] + cache[i-2]
                         + (seq_t)2 * cache[i-3] + (seq_t)3 * cache[i-6];
                seq_t neg = (seq_t)13 * cache[i-4] + (seq_t)7 * cache[i-5];
                cache[i] = pos - neg;   /* safe: pos > neg for all n >= 7 */
            }
        }
        cache_len = n;
    }

    return cache[n-1];
}
