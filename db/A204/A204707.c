/*
 * A204707  Number of (n+1) X 3 0..1 arrays with the permanents of all 2 X 2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 4*a(n-3), with a(1)=13, a(2)=33,
 *            a(3)=81
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to memoize computed values.  Index 0 is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 1) {
        return (seq_t)0;  /* Undefined for n < 1; return 0. */
    }

    /* Initialise cache with the first three terms if not yet done. */
    if (!cache) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));   /* indices 0..3 */
        if (!cache) return (seq_t)0;                  /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)13;
        cache[2] = (seq_t)33;
        cache[3] = (seq_t)81;
        cache_size = 3;
    }

    /* Grow cache if the requested index is beyond current size. */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;              /* allocation failed */
        cache = new_cache;
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            seq_t val = 2 * cache[k - 1] + 3 * cache[k - 2];
            val -= 4 * cache[k - 3];
            cache[k] = val;
        }
        cache_size = n;
    }

    return cache[n];
}
