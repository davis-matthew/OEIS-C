/*
 * A063207  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          33 ).
 * https://oeis.org/A063207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063207:
 * a(n) = a(n-2) + a(n-3) - a(n-5)  for n > 6
 * a(1) = 1, a(2) = 6, a(3) = 8, a(4) = 12, a(5) = 14, a(6) = 20
 *
 * The function memoises all computed terms in a static array so that
 * subsequent calls are O(1).  It grows the array only when it must
 * accommodate a larger index.  Values are kept in the built‑in type
 * seq_t; the recurrence never yields a value that does not fit
 * into seq_t for the ranges considered.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of elements allocated (indices 0 .. cache_len-1) */

    /* initialise cache with the first 6 terms if not yet done */
    if (cache_len == 0) {
        const idx_t init = 7;               /* need indices 0..6 */
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (cache == NULL) abort();         /* allocation failed */
        cache_len = init;
        cache[0] = 0;        /* dummy for 0‑th index, unused */
        cache[1] = 1;
        cache[2] = 6;
        cache[3] = 8;
        cache[4] = 12;
        cache[5] = 14;
        cache[6] = 20;
    }

    /* if the cache already covers the requested n, return it */
    if ((size_t)n + 1 <= (size_t)cache_len) {
        return cache[n];
    }

    /* grow the cache to accommodate n (indices 0 .. n) */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) abort();          /* allocation failed */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* i >= 7 here, so all indices below i exist in cache */
        cache[i] = cache[i - 2] + cache[i - 3] - cache[i - 5];
    }

    cache_len = (idx_t)new_len;
    return cache[n];
}
