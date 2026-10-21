/*
 * A065102  a(0) = c, a(1) = p*c^3; a(n+2) = p*c^2*a(n+1) - a(n), for p = 2, c
 *          = 3.
 * https://oeis.org/A065102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 3, a(1) = 54, a(n+2) = 18*a(n+1) - a(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored      */
    static size_t cache_cap = 0;   /* allocated capacity of cache array      */

    /* Grow the cache if the required index is not yet present. */
    if (cache_len <= (size_t)n)
    {
        /* Determine the minimum capacity needed. */
        size_t needed = (size_t)n + 1;
        if (cache_cap < needed)
        {
            size_t new_cap = needed > 2 ? needed : 2;       /* at least 2 entries */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;                            /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute terms from the current length up to n. */
        for (size_t i = cache_len; i <= (size_t)n; ++i)
        {
            seq_t val;
            if (i == 0)
                val = (seq_t)3;
            else if (i == 1)
                val = (seq_t)54;
            else
                val = (seq_t)18 * cache[i - 1] - cache[i - 2];
            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
