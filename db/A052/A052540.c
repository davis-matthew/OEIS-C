/*
 * A052540  Expansion of (1-x)/(1-2*x-x^3+x^4).
 * https://oeis.org/A052540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)=2*a(n-1)+a(n-3)-a(n-4), a(0)=1, a(1)=1, a(2)=2, a(3)=5
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 60, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* memoised terms */
    static idx_t cache_len = 0;       /* number of cached terms */

    /* initialise the cache with the first four values */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure – unlikely for the test harness */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
    }

    /* existing value requested */
    if (n < cache_len)
        return cache[n];

    /* grow the cache to include index n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;  /* allocation failure – avoid crash */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 2 * cache[i - 1];
        val += cache[i - 3];
        val -= cache[i - 4];
        cache[i] = val;
    }

    cache_len = new_len;
    return cache[n];
}
