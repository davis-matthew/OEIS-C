/*
 * A050231  a(n) is the number of n-tosses having a run of 3 or more heads for
 *          a fair coin (i.e., probability is a(n)/2^n).
 * https://oeis.org/A050231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - a(n-3) - 2*a(n-4) with a(1)=0,
 *            a(2)=0, a(3)=1, a(4)=3
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;

    /* initialise cache with base terms */
    if (!cache) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));      /* indices 0..4 used */
        cache[0] = 0;                                   /* unused, for safety */
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 1;
        cache[4] = 3;
        cache_max = 4;
    }

    /* if requested n already computed, return it */
    if (n <= cache_max)
        return cache[n];

    /* extend cache to size n+1 (indices 0..n) */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) /* allocation failed – silently ignore, caller will get garbage */
        return cache[n];
    cache = tmp;

    /* compute new terms using recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t term = 3 * cache[i - 1];
        term -= cache[i - 2];
        term -= cache[i - 3];
        term -= 2 * cache[i - 4];
        cache[i] = term;
    }
    cache_max = n;
    return cache[n];
}
