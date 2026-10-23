/*
 * A153819  Linear recurrence with a(n) = 3a(n-1) - a(n-2) + 2 = 4a(n-1) -
 *          4a(n-2) + a(n-3). Full sequence for A153466.
 * https://oeis.org/A153819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + 2 with a(0)=16, a(1)=34 for n >= 0
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache keeps computed values for future calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with first two terms if empty */
    if (cache_len == 0)
    {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;      /* graceful failure if malloc fails */
        cache[0] = 16;
        cache[1] = 34;
    }

    /* extend cache if necessary */
    if (n + 1 > cache_len)
    {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (!cache) return 0;      /* graceful failure if realloc fails */

        for (idx_t i = old_len; i <= n; ++i)
        {
            /* recurrence: a(i) = 3*a(i-1) - a(i-2) + 2 */
            cache[i] = 3 * cache[i - 1] - cache[i - 2] + 2;
        }
    }

    return cache[n];
}
