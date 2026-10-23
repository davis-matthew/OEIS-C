/*
 * A034300  a(n) = n-th quintic factorial number divided by 3.
 * https://oeis.org/A034300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*n-2)*a(n-1), a(1) = 1 (equivalently, a(n) =
 *            Π_{j=1..n}(5*j-2) / 3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, return 0 as guard */

    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cacheSize = 0;   /* number of stored terms */

    /* extend cache if needed */
    if (n > cacheSize)
    {
        idx_t newSize = n;
        seq_t *tmp = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
        cache = tmp;

        if (cacheSize == 0)
        {
            /* initialise the first term */
            cache[0] = 1;
            cacheSize = 1;
        }

        /* compute new terms iteratively */
        for (idx_t k = cacheSize + 1; k <= n; ++k)
        {
            /* multiplier = 5*k - 2 (fits in idx_t for n up to about 2^63/5) */
            seq_t mult = (seq_t)(5 * k - 2);
            cache[k - 1] = cache[k - 2] * mult;
        }

        cacheSize = n;
    }

    return cache[n - 1];
}
