/*
 * A066534  Total number of walks with length > 0 in the Hasse diagram of a
 *          Boolean algebra of order n.
 * https://oeis.org/A066534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice, walk
 * Formula:   a(n) = n * ( a(n-1) + 2^(n-1) ), a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent storage for memoization and powers of 2   */
    static seq_t *cache      = NULL;    /* a[0..max] */
    static idx_t  cache_size = 0;        /* number of allocated entries */
    static idx_t  last       = 0;        /* highest index computed so far */
    static seq_t  pow2       = 0;        /* 2^last */

    /* ----- initialization ----- */
    if (cache == NULL) {
        cache_size = 1;                 /* store a(0) */
        cache      = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0]   = (seq_t)0;           /* a(0) = 0 */
        pow2       = (seq_t)1;           /* 2^0 for next computation */
        last       = 0;
    }

    /* Expand cache if necessary */
    if ((idx_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* cache[i] = i * (cache[i-1] + 2^(i-1))   */
        seq_t term = cache[i-1] + pow2;
        cache[i] = (seq_t)i * term;

        /* Prepare 2^i for next iteration */
        pow2 = pow2 + pow2;     /* double: pow2 *= 2 */
    }
    last = n;

    return cache[n];
}
