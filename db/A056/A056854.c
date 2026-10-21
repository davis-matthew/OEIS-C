/*
 * A056854  a(n) = Lucas(4*n).
 * https://oeis.org/A056854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 7*a(n-1) - a(n-2), a(0)=2, a(1)=7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lucas(4*n) can be computed with this linear recurrence. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with first two terms. */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        cache[0] = 2;
        cache[1] = 7;
    }

    /* Return precomputed value if already known. */
    if (n < cache_len)
        return cache[n];

    /* Extend cache up to n using the recurrence. */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache = tmp;
    for (idx_t i = cache_len; i <= n; ++i)
        cache[i] = 7 * cache[i - 1] - cache[i - 2];
    cache_len = n + 1;

    return cache[n];
}
