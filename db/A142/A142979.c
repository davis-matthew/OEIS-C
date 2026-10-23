/*
 * A142979  a(1) = 1, a(2) = 3, a(n+2) = 3*a(n+1) + (n+1)^2*a(n).
 * https://oeis.org/A142979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(2) = 3,
   a(n+2) = 3*a(n+1) + (n+1)^2*a(n)          */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cache[0] holds a(1) */
    static idx_t  cache_sz = 0;     /* number of terms stored */

    if (n == 0)   /* undefined – return 0 */
        return 0;

    /* extend cache if necessary */
    if (n > cache_sz) {
        /* realloc may need to allocate a larger block */
#ifndef __cplusplus
        cache = realloc(cache, n * sizeof(seq_t));
#else
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
#endif
        if (!cache)
            abort();

        /* initialise missing base terms */
        if (cache_sz == 0) {          /* first time */
            cache[0] = 1;             /* a(1) */
            if (n >= 2)
                cache[1] = 3;         /* a(2) */
            cache_sz = (n >= 2) ? 2 : 1;
        } else if (cache_sz == 1) {   /* we had only a(1) before */
            cache[1] = 3;             /* a(2) */
            cache_sz = 2;
        }

        /* compute remaining terms using the recurrence */
        for (idx_t i = cache_sz; i < n; ++i) {
            /* i corresponds to term a(i+1), i >= 2 here */
            /* a(i+1) = 3*a(i) + i^2*a(i-1) */
            cache[i] = (seq_t)3 * cache[i - 1]
                     + (seq_t)i * (seq_t)i * cache[i - 2];
        }

        cache_sz = n;
    }

    return cache[n - 1];
}
