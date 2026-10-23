/*
 * A198900  Number of n X 2 0..4 arrays with values 0..4 introduced in row
 *          major order and no element equal to any horizontal or vertical
 *          neighbor.
 * https://oeis.org/A198900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 17*a(n-1) - 55*a(n-2) + 39*a(n-3)   (n ≥ 4)
   Base values: a(1)=1, a(2)=4, a(3)=33 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined, return 0 */

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest index currently stored */

    /* Initialise cache with the first three terms */
    if (cache_len < 3)
    {
        cache_len = 3;
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        /* cache[0] is unused to keep indices 1‑based */
        cache[0] = 0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)33;
    }

    /* Resize and compute any missing terms up to n */
    if (n > cache_len)
    {
        /* Allocate a larger array */
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        memcpy(new_cache, cache, (cache_len + 1) * sizeof(seq_t));

        /* Compute terms from cache_len+1 … n */
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            /* Base cases already known for i == 1,2,3 */
            if (i == 1)
                new_cache[i] = (seq_t)1;
            else if (i == 2)
                new_cache[i] = (seq_t)4;
            else if (i == 3)
                new_cache[i] = (seq_t)33;
            else
                new_cache[i] = 17 * new_cache[i - 1]
                             - 55 * new_cache[i - 2]
                             + 39 * new_cache[i - 3];
        }

        /* Replace old cache */
        free(cache);
        cache     = new_cache;
        cache_len = n;
    }

    return cache[n];
}
