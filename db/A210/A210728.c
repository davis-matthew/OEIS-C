/*
 * A210728  a(n) = a(n-1) + a(n-2) + n + 2 with n>1, a(0)=1, a(1)=2.
 * https://oeis.org/A210728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + n + 2, a(0)=1, a(1)=2 (recurrence)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Handle first two terms explicitly */
    if (n == 0) return 1;
    if (n == 1) return 2;

    /* On first call, allocate storage for the base cases */
    if (cache == NULL)
    {
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0;           /* allocation failure – unlikely in tests */
        cache[0] = 1;
        cache[1] = 2;
    }

    /* Grow the cache if the requested index is beyond current size */
    if (n >= cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return 0;             /* allocation failure – unlikely */
        cache = tmp;

        /* Compute missing terms using the recurrence */
        for (idx_t i = cache_size; i < new_size; ++i)
        {
            /* cache[i] = cache[i-1] + cache[i-2] + i + 2;  */
            cache[i] = cache[i - 1] + cache[i - 2] + (seq_t)i + 2;
        }
        cache_size = new_size;
    }

    return cache[n];
}
