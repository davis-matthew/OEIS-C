/*
 * A120464  a(n) = 5*a(n-1)+a(n-2)-2*a(n-3).
 * https://oeis.org/A120464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)+a(n-2)-2*a(n-3), with a(0)=0, a(1)=2, a(2)=11
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* memoised sequence values */
    static idx_t cache_len = 0;        /* number of elements stored */

    /* Ensure the cache can hold indices 0..n */
    if (cache_len <= n) {
        /* Allocate or grow the cache to n+1 elements */
        cache_len = n + 1;
        cache = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
        /* If this is the first allocation, the returned pointer may
           be NULL for a zero-sized allocation; handle that case. */
        if (cache == NULL && cache_len > 0) /* should never happen in sane envs */
            return (seq_t)0;   /* fallback: undefined */
    }

    /* Initialise base cases */
    if (cache_len >= 1) cache[0] = (seq_t)0;
    if (cache_len >= 2) cache[1] = (seq_t)2;
    if (cache_len >= 3) cache[2] = (seq_t)11;

    /* Compute missing terms up to n (if any) */
    for (idx_t i = 3; i <= n; ++i) {
        cache[i] = 5 * cache[i-1] + cache[i-2] - 2 * cache[i-3];
    }

    return cache[n];
}
