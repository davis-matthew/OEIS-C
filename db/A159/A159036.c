/*
 * A159036  a(0)=0, a(1)=1, a(2)=4, a(3)=13; thereafter
 *          a(n+3)=4*a(n+2)-4*a(n+1)+2*a(n) for n>=1.
 * https://oeis.org/A159036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=4, a(3)=13; a(n+3) = 4*a(n+2) - 4*a(n+1) +
 *            2*a(n) for n >= 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation is performed iteratively with memoisation to keep the
   values for future calls.  The recurrence is evaluated only once
   for each index.  The implementation works with any of the supplied
   integer or arbitrary‑precision types, as the cache is allocated
   dynamically with malloc/realloc. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of cached elements */

    /* Expand cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)          /* memory allocation failed */
            return 0;     /* benign error handling */
        cache = tmp;

        /* initialise new entries */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0)                 cache[i] = 0;
            else if (i == 1)            cache[i] = 1;
            else if (i == 2)            cache[i] = 4;
            else if (i == 3)            cache[i] = 13;
            else                        cache[i] = 4 * cache[i-1]
                                          - 4 * cache[i-2]
                                          + 2 * cache[i-3];
        }
        cache_size = new_size;
    }
    return cache[(size_t)n];
}
