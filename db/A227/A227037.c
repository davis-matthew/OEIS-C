/*
 * A227037  Partial sums of A013999.
 * https://oeis.org/A227037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+3)*a(n-1) - 2*(n+1)*a(n-2) + (n+1)*a(n-3) for n>2,
 *            a(0) = 1, a(1) = 2, a(2) = 4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of elements stored (0 .. cache_size-1) */

    /* already computed ? */
    if (n < cache_size)
        return cache[n];

    /* grow cache to size n+1 */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = NULL;

    if (cache == NULL) {
        new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    } else {
        new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    }
    if (!new_cache) {
        /* allocation failed – free existing cache and give up */
        free(cache);
        cache = NULL;
        cache_size = 0;
        return 0;
    }
    cache = new_cache;

    /* compute missing values */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 2) {
            /* base values: 1, 2, 4 */
            cache[i] = ((seq_t)1 << i);          /* 2^i fits in seq_t for i <= 2 */
        } else {
            /* recurrence */
            cache[i] =
                (seq_t)(i + 3) * cache[i - 1]
                - (seq_t)2 * (seq_t)(i + 1) * cache[i - 2]
                + (seq_t)(i + 1) * cache[i - 3];
        }
    }

    cache_size = n + 1;
    return cache[n];
}
