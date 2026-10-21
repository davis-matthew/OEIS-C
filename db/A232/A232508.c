/*
 * A232508  Number of (n+1) X (1+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally,
 *          diagonally or antidiagonally, with no adjacent elements equal.
 * https://oeis.org/A232508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-2)+a(n-3) for n>=4;
   a(1)=4, a(2)=4, a(3)=8. */

#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;

    /* handle the base cases explicitly */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;

    if (n <= cache_max) /* already computed */
        return cache[n];

    /* make sure we have enough space to store up to n */
    ensure_cache(n, &cache, &cache_cap);

    if (cache_max == 0) {        /* initialise the first three values */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
        cache_max = 3;
    }

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3];
    }
    cache_max = n;

    return cache[n];
}
