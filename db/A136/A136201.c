/*
 * A136201  a(0)=a(1)=a(2)=0, a(3)=1, and a(n) = 2*a(n-1) + 4*a(n-2) -
 *          6*a(n-3) - 3*a(n-4) for n > 3.
 * https://oeis.org/A136201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 6*a(n-3) - 3*a(n-4) Initial values:
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=1
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 52, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial base cases without allocating memory */
    if (n < 4) {
        static const seq_t base[4] = {0, 0, 0, 1};
        return base[n];
    }

    /* Static cache to memoise previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Initialise the cache on the first call that requires it */
    if (cache == NULL) {
        cache_cap = 4;                     /* indices 0..3 are needed initially */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;        /* allocation failure -> return 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
    }

    /* Extend the cache if the requested index is beyond the current capacity */
    if ((idx_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;        /* ensure we can index up to n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;            /* allocation failure -> return 0 */
        cache = tmp;

        /* Compute entries from the old capacity up to and including n */
        for (idx_t i = (idx_t)cache_cap; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i-1]
                     + (seq_t)4 * cache[i-2]
                     - (seq_t)6 * cache[i-3]
                     - (seq_t)3 * cache[i-4];
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
