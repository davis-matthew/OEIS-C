/*
 * A162671  For n even a(n) = a(n-1) + a(n-2), for n odd a(n) = 100*a(n-1) +
 *          a(n-2), with a(0) = 0, a(1) = 1.
 * https://oeis.org/A162671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1; for even n>1 a(n)=a(n-1)+a(n-2); for odd n>1 *
 *            a(n)=100*a(n-1)+a(n-2). This function memoises results in a
 *            static array * so it works correctly for calls in any order and
 *            runs in O(n) time for * n up to about 10 000
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;   /* allocated capacity */
    static idx_t up_to       = 0;   /* largest index with value in cache */

    /* initialise for the first call */
    if (cache == NULL) {
        cache_size = 2;
        cache      = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) {
            return 0; /* fallback on allocation failure */
        }
        cache[0] = 0;
        cache[1] = 1;
        up_to    = 1;
    }

    /* grow the cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL) {
            return 0; /* fallback */
        }
        cache   = tmp;
        for (idx_t i = cache_size; i < new_size; ++i) {
            cache[i] = 0; /* initialise new slots */
        }
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    if (n > up_to) {
        for (idx_t i = up_to + 1; i <= n; ++i) {
            if ((i & 1) == 0) {     /* even i */
                cache[i] = cache[i - 1] + cache[i - 2];
            } else {                /* odd i */
                cache[i] = 100 * cache[i - 1] + cache[i - 2];
            }
        }
        up_to = n;
    }

    return cache[n];
}
