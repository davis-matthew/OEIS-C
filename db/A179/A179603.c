/*
 * A179603  Eight white kings and one red king on a 3 X 3 chessboard. G.f.: (1
 *          + 3*x)/(1 - 3*x - 7*x^2).
 * https://oeis.org/A179603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 7*a(n-2), with a(0)=1, a(1)=6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* Number of elements currently stored */

    /* Initialise the cache with the first two values if it is empty */
    if (!cache) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* Allocation failed – return zero */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache_len = 2;
    }

    /* If the requested index is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache to accommodate the new index */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                  /* Allocation failed – return zero */
    cache = tmp;

    /* Compute the missing values using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i - 1] + (seq_t)7 * cache[i - 2];
    }
    cache_len = new_len;

    return cache[n];
}
