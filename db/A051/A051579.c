/*
 * A051579  a(n) = (2*n+5)!!/5!!, related to A001147 (odd double factorials).
 * https://oeis.org/A051579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+5)!!/5!! (OEIS A051579) Recurrence: a(0) = 1 a(n) =
 *            (2*n+5) * a(n-1) for n ≥ 1 The implementation memoizes computed
 *            values so that calls in any order remain O(n) total time
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache   = NULL;   /* cache[0] = a(0) ... cache[n] = a(n) */
    static idx_t  cache_len = 0;    /* number of elements allocated in cache */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_len = 1;                     /* capacity for a(0) */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL) return (seq_t)0; /* out of memory: return 0 as error */
        cache[0] = (seq_t)1;               /* a(0) = 1 */
    }

    /* if n is already known, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* grow cache to accommodate a(n) */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (tmp == NULL)
        return (seq_t)0; /* out of memory */
    cache = tmp;

    /* compute missing terms from old_len to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence factor (2*i+5) */
        seq_t factor = (seq_t)(2 * (seq_t)i + 5);
        cache[i] = factor * cache[i - 1];
    }
    cache_len = new_len;

    return cache[n];
}
