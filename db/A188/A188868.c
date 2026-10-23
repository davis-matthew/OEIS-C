/*
 * A188868  Number of nX3 binary arrays without the pattern 0 0 0
 *          antidiagonally or horizontally.
 * https://oeis.org/A188868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 3*a(n-2) - 5*a(n-3) + 2*a(n-4) initial values:
 *            a(1)=7, a(2)=49, a(3)=316, a(4)=2032
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑based cache: cache[i] holds a(i).  cache_size is the largest
       index that has been computed.  cache[0] is unused but required
       for the recurrence. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) /* out of bounds – not used by OEIS */
        return 0;

    /* Ensure the cache is large enough.  Always allocate room for at
       least the first four terms, even if n < 4, to keep cache[4] well
       defined for later calls. */
    idx_t new_size = (n >= 4) ? n + 1 : 5;   /* indices 0..new_size-1 */
    if (cache_size == 0 || new_size > cache_size)
    {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;

        /* Initialize the first four terms once.  This guarantees that
           cache[4] is always defined, so later calls can fill only
           missing terms using the recurrence. */
        if (cache_size == 0)
        {
            cache[1] = (seq_t)7;
            cache[2] = (seq_t)49;
            cache[3] = (seq_t)316;
            cache[4] = (seq_t)2032;
            cache_size = 4;
        }
    }

    /* Compute any terms that are now required */
    if (cache_size < n)
    {
        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            cache[i] =
                (seq_t)7 * cache[i - 1]
              - (seq_t)3 * cache[i - 2]
              - (seq_t)5 * cache[i - 3]
              + (seq_t)2 * cache[i - 4];
        }
        cache_size = n;
    }

    return cache[n];
}
