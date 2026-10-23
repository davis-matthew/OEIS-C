/*
 * A155859  a(n) = (1/162)*(61*10^n + 18*n + 20).
 * https://oeis.org/A155859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=4, a(2)=38, a(3)=377;
   a(n)=12*a(n-1)-21*a(n-2)+10*a(n-3) for n≥4 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache of computed values, 1‑based: cache[0] holds a(1) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0) return 0;           /* offset below 1 is irrelevant */

    if (n > cache_len) {
        /* extend cache to size n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cache_len; i < n; ++i) {
            if (i == 0)
                cache[0] = (seq_t)4;
            else if (i == 1)
                cache[1] = (seq_t)38;
            else if (i == 2)
                cache[2] = (seq_t)377;
            else
                cache[i] = (seq_t)(12 * cache[i-1] - 21 * cache[i-2] + 10 * cache[i-3]);
        }
        cache_len = n;
    }
    return cache[n-1];
}
