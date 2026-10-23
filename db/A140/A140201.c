/*
 * A140201  Partial sums of A140081.
 * https://oeis.org/A140201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) for n > 4; Initial values: a(0)
 *            = 0, a(1) = 1, a(2) = 2, a(3) = 4, a(4) = 4
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of entries currently cached */

    /* If the cache is too small, grow it and compute the missing terms */
    if (cache_size <= n) {
        idx_t newsize = n + 1;
        if (newsize < 5) newsize = 5;                     /* at least 5 entries */

        seq_t *tmp = (seq_t*)realloc(cache, (size_t)newsize * sizeof(seq_t));
        if (!tmp) return 0;                               /* allocation failure */
        cache = tmp;

        /* Initialise the first five values once */
        if (cache_size == 0) {
            cache[0] = 0;
            cache[1] = 1;
            cache[2] = 2;
            cache[3] = 4;
            cache[4] = 4;
        }

        /* Compute any new terms using the recurrence */
        for (idx_t i = cache_size; i < newsize; ++i) {
            if (i <= 4) continue;                         /* already set */
            cache[i] = cache[i - 1] + cache[i - 4] - cache[i - 5];
        }

        cache_size = newsize;
    }

    /* Return the requested value */
    return cache[(size_t)n];
}
