/*
 * A160843  Number of lines through at least 2 points of a 3 X n grid of
 *          points.
 * https://oeis.org/A160843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) for n > 5 Initial values:
 *            a(0)=0, a(1)=1, a(2)=11, a(3)=20, a(4)=35, a(5)=52
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of elements already stored */

    /* compute up to index n if necessary */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        /* allocate or grow the cache */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* base values for n = 0..5 */
        static const seq_t base[6] = { 0, 1, 11, 20, 35, 52 };

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 6) {
                cache[i] = base[i];
            } else {
                seq_t v1 = cache[i - 1];
                seq_t v2 = cache[i - 3];
                seq_t v3 = cache[i - 4];
                cache[i] = v1 + v1;      /* 2*a(i-1) */
                cache[i] -= v2 + v2;     /* -2*a(i-3) */
                cache[i] += v3;          /* +a(i-4) */
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
