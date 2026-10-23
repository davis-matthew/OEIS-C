/*
 * A049876  a(n) = Sum{a(k): k=0,1,2,...,n-4,n-2,n-1}; a(n-3) is not a
 *          summand; initial terms are 2,3,4.
 * https://oeis.org/A049876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ { a(k) : 0 ≤ k ≤ n-1, k ≠ n-3 } with a(0)=2, a(1)=3,
 *            a(2)=4. Equivalent linear recurrence:
 *            a(n)=2*a(n-1)-a(n-3)+a(n-4)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;        /* number of terms stored (next free index) */
    static seq_t running_sum = 0;      /* sum of cache[0 .. cache_len-1] */

    /* initialise cache with first three terms on first call */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)4;
        running_sum = cache[0] + cache[1] + cache[2];
    }

    /* if requested index already in cache, return it */
    if (n < cache_len)
        return cache[n];

    /* expand cache to include n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = sum_{k=0}^{i-1} a(k) - a(i-3)  (i >= 3 here) */
        seq_t val = running_sum - cache[i - 3];
        cache[i] = val;
        running_sum += val;
    }

    cache_len = new_len;
    return cache[n];
}
