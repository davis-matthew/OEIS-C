/*
 * A161944  a(n) = ((4+sqrt(2))*(3+sqrt(2))^n + (4-sqrt(2))*(3-sqrt(2))^n)/4.
 * https://oeis.org/A161944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2), a(0) = 2, a(1) = 7
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* handle the two initial values */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)7;

    /* expand cache if necessary */
    if (n >= cache_size)
    {
        idx_t new_size = (cache_size == 0 ? 2 : cache_size * 2);
        while (new_size <= n) new_size <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failure -- abort */
            exit(EXIT_FAILURE);
        }
        cache = tmp;

        /* initialise first two terms if not already done */
        if (cache_size == 0)
        {
            cache[0] = (seq_t)2;
            cache[1] = (seq_t)7;
            cache_size = 2;
        }

        /* compute new terms up to n */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            cache[i] = (seq_t)6 * cache[i - 1] - (seq_t)7 * cache[i - 2];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
