/*
 * A080421  a(n) = (n+1)*(n+2)*(n+9)*3^n/18.
 * https://oeis.org/A080421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 33, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A080421
   Formula: a(n) = (n+1)*(n+2)*(n+9)*3^n/18
   Recurrence: a(0)=1, a(1)=10, a(2)=66, a(3)=360,
               a(n)=12*a(n-1) - 54*a(n-2) + 108*a(n-3) - 81*a(n-4)  for n≥4
 */

#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache – grows only when a larger index is needed */
    static seq_t *cache      = NULL;   /* array of computed terms        */
    static idx_t   cache_len =  0;     /* number of terms currently stored */

    /* initialise the cache with the first four values on first use */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL)    /* allocation failure – give up and return 0 */
            return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)66;
        cache[3] = (seq_t)360;
    }

    /* extend the cache if the requested index is beyond what we have */
    if (cache_len <= n) {
        idx_t new_len = n + 1;                   /* new size (including index n) */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)                         /* memory exhaustion */
            return cache[n];                     /* best we can do */
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 4)                       /* already initialised */
                continue;
            cache[i] = 12 * cache[i-1]
                     - 54 * cache[i-2]
                     +108 * cache[i-3]
                     - 81 * cache[i-4];
            /* All operations are on seq_t, which is the correct width for
               the building configuration (uint8/16/32/64/__int128).  */
        }
        cache_len = new_len;
    }

    return cache[n];
}
