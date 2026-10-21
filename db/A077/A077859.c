/*
 * A077859  Expansion of g.f. (1 - x)^(-1)/(1 - 2*x + 2*x^2 + x^3).
 * https://oeis.org/A077859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 1 + a(n-1) - a(n-2), with a(0) = 1, a(1) = 3. Also
 *            a(2)=5, a(3)=6
 * Range:     u8 n <= 253, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n < cache_len)
        return cache[n];

    /* enlarge cache to hold values up to n */
    if (cache_len == 0) {
        cache_len = n + 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure – unlikely */
    } else if ((idx_t)cache_len <= n) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!cache) return 0;
        cache_len = n + 1;
    }

    /* initialise base values */
    if (cache_len > 0 && cache[0] != 1) cache[0] = 1;
    if (cache_len > 1 && cache[1] != 3) cache[1] = 3;
    if (cache_len > 2 && cache[2] != 5) cache[2] = 5;
    if (cache_len > 3 && cache[3] != 6) cache[3] = 6;

    /* compute any values that are still missing */
    for (idx_t i = 4; i <= n; ++i) {
        /* Use signed intermediate to avoid unsigned underflow */
        long long val = (long long)i + 1 + (long long)cache[i-1] - (long long)cache[i-2];
        cache[i] = (seq_t)val;
    }

    return cache[n];
}
