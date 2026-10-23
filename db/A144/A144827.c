/*
 * A144827  Partial products of successive terms of A017029; a(0)=1.
 * https://oeis.org/A144827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n-1} (4 + 7*k), a(0) = 1 (equivalently a(n)
 *            = a(n-1) * (4 + 7*(n-1)) for n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* a(0) … a(cache_size-1) */
    static idx_t cache_size = 0;        /* number of stored terms */

    /* Grow the cache if the desired n is beyond current range. */
    if (n + 1 > cache_size) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp;
        if (cache == NULL) {
            tmp = (seq_t *)malloc(new_sz * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        }
        if (tmp == NULL) return (seq_t)0;      /* allocation failed */
        cache = tmp;
    }

    /* Compute all terms that are not yet cached. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else {
            seq_t term = (seq_t)(7 * (i - (idx_t)1) + (idx_t)4);
            cache[i] = cache[i - (idx_t)1] * term;
        }
    }

    /* Update the number of cached terms. */
    cache_size = n + 1;

    return cache[n];
}
