/*
 * A088981  a(n+2) = a(n+1) + a(n) - (2*n + 1) where a(0)=7, a(1)=11.
 * https://oeis.org/A088981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = a(n-1) + a(n-2) - (2*(n-2)+1)
 * Initial terms: a(0) = 7, a(1) = 11
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure the two base values are stored */
    if (cache_size < 2) {
        seq_t *tmp = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* out of memory – abort silently */
        cache = tmp;
        cache[0] = 7;
        cache[1] = 11;
        cache_size = 2;
    }

    /* Quick return if we already have the value */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to fit the requested index */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* out of memory – abort silently */
    cache = tmp;

    /* Compute new terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t subtract = (seq_t)(2 * (i - 2) + 1);   /* (2*n-3) for i = n+2 */
        cache[i] = cache[i - 1] + cache[i - 2] - subtract;
    }

    cache_size = n + 1;
    return cache[n];
}
