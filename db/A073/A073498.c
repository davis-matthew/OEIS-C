/*
 * A073498  Binomial transform of A073145.
 * https://oeis.org/A073498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 2*a(n-3) with initial values
 *            a(0)=3, a(1)=4, a(2)=4
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 28, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t cached = 0;      /* number of entries currently stored (equals cache size) */

    /* request at least n+1 entries (0 … n) */
    if (n + 1 > cached)
    {
        /* enlarge cache */
        idx_t old = cached;                    /* number of already stored values */
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_cache)
        {
            abort();                          /* allocation failure */
        }

        /* copy old values */
        for (idx_t i = 0; i < old; ++i)
        {
            new_cache[i] = cache[i];
        }

        /* free previous buffer */
        free(cache);
        cache = new_cache;
        cached = n + 1;                        /* new size */

        /* compute newly required terms */
        for (idx_t i = old; i <= n; ++i)
        {
            if (i == 0)  cache[i] = (seq_t)3;
            else if (i == 1)  cache[i] = (seq_t)4;
            else if (i == 2)  cache[i] = (seq_t)4;
            else
            {
                /* recurrence: a(i) = 4*a(i-1) - 6*a(i-2) + 2*a(i-3) */
                cache[i] = (seq_t)4 * cache[i - 1]
                           - (seq_t)6 * cache[i - 2]
                           + (seq_t)2 * cache[i - 3];
            }
        }
    }
    else
    {
        /* compute remaining entries without reallocating */
        for (idx_t i = cached; i <= n; ++i)
        {
            if (i == 0)  cache[i] = (seq_t)3;
            else if (i == 1)  cache[i] = (seq_t)4;
            else if (i == 2)  cache[i] = (seq_t)4;
            else
            {
                cache[i] = (seq_t)4 * cache[i - 1]
                           - (seq_t)6 * cache[i - 2]
                           + (seq_t)2 * cache[i - 3];
            }
        }
        cached = n + 1;                           /* update size */
    }

    return cache[n];
}
