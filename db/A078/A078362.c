/*
 * A078362  A Chebyshev S-sequence with Diophantine property.
 * https://oeis.org/A078362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - a(n-2), a(-1)=0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 13*a(0) - a(-1) = 13, then use the recurrence for n ≥ 2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t cache_size = 0;
    static seq_t *cache = NULL;

    /* already computed? */
    if (n < cache_size)
        return cache[n];

    /* grow cache to hold indices up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)                    /* allocation failure – return 0 */
        return (seq_t)0;
    cache = new_cache;

    /* initialise base case if not already */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;          /* a(0) */
        cache_size = 1;
    }

    /* compute new terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)13 * cache[0];   /* a(1) = 13*a(0) – 0 */
        } else {
            cache[i] = (seq_t)13 * cache[i - 1] - cache[i - 2];
        }
    }

    cache_size = n + 1;
    return cache[n];
}
