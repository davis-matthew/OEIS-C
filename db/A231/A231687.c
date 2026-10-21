/*
 * A231687  a(n) = Sum_{i=0..n} digsum_9(i)^4, where digsum_9(i) = A053830(i).
 * https://oeis.org/A231687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digsum_9(i)^4, where digsum_9(i) is the sum
 *            of the base‑9 digits of i
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of cumulative sums.  cache[k] stores a(k). */
    static seq_t *cache = NULL;          /* pointer to cache array          */
    static size_t cache_size = 0;        /* current allocated size          */
    static idx_t max_n = 0;              /* highest n computed so far       */

    /* Handle n == 0 directly. */
    if (n == 0)
        return (seq_t)0;

    /* Initialise cache if necessary. */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 1);
        if (cache == NULL)
            exit(EXIT_FAILURE);
        cache[0] = (seq_t)0;
        cache_size = 1;
        max_n = 0;
    }

    /* Expand the cache if the requested n is beyond the current size. */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL)
            exit(EXIT_FAILURE);
        cache = new_cache;
        cache_size = new_size;

        /* Compute missing entries from max_n+1 up to n. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* Compute the base‑9 digit sum of i. */
            seq_t ds = (seq_t)0;
            idx_t temp = i;
            while (temp > 0) {
                ds += (seq_t)(temp % 9);
                temp /= 9;
            }

            /* Compute ds^4.  Use two squarings to avoid an extra multiplication. */
            seq_t ds2 = ds * ds;
            seq_t ds4 = ds2 * ds2;

            /* Cumulative sum. */
            cache[i] = cache[i - 1] + ds4;
        }
        max_n = n;
    }

    return cache[n];
}
