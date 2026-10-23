/*
 * A124745  Expansion of (1+x)/(1-x^2+x^3).
 * https://oeis.org/A124745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-2) - a(n-3), a(0) = a(1) = a(2) = 1
 * Range:     i8 n <= 25, i16 n <= 45, i32 n <= 84, i64 n <= 163,
 *            i128 n <= 321, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised cache of previously computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of slots stored (>= 0) */

    /* Base cases are trivial for indices 0..2. */
    if (n < 3u)
        return (seq_t)1;

    /* If we do not yet have a value for n, grow the cache. */
    if (cache_len <= n)
    {
        /* At least n+1 entries are required. */
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            abort();    /* allocation failure */

        /* Initialise the first three terms on the very first allocation. */
        if (cache_len == 0)
        {
            tmp[0] = (seq_t)1;
            tmp[1] = (seq_t)1;
            tmp[2] = (seq_t)1;
            cache_len = 3;
        }

        /* Compute any missing terms using the recurrence. */
        for (idx_t i = cache_len; i <= n; ++i)
            tmp[i] = tmp[i - 2] - tmp[i - 3];

        /* Commit the new cache and its length. */
        cache      = tmp;
        cache_len  = new_len;
    }

    return cache[n];
}
