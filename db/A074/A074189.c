/*
 * A074189  a(1) = 1, a(2) = 2; for n > 2, a(n) = {a(n-1) +a(n+1)}/n or a(n+1)
 *          = n*a(n)-a(n-1).
 * https://oeis.org/A074189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-1)*a(n-1) - a(n-2) Initial values: a(0) = -1, a(1) =
 *            1, a(2) = 2 Recurrence derived from a(n+2) = (n+1)*a(n+1) - a(n)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of values stored (i.e. next free index) */

    /* Initialise cache on first call */
    if (cache == NULL)
    {
        cache_len = 3;                     /* we will store indices 0,1,2 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;       /* allocation failure -> return 0 (unused) */

        cache[0] = -1;     /* a(0) */
        cache[1] =  1;     /* a(1) */
        cache[2] =  2;     /* a(2) */
    }

    /* Return directly if already computed */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache if necessary */
    if (n >= cache_len)
    {
        idx_t new_len = n + 1;                 /* index n will be stored at cache[n] */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return 0;            /* allocation failure -> return 0 (unused) */
        cache = tmp;
        /* Compute the missing values iteratively */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            /* a(i) = (i-1)*a(i-1) - a(i-2) */
            cache[i] = ((seq_t)(i - 1)) * cache[i - 1] - cache[i - 2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
