/*
 * A015545  Expansion of x/(1 - 5*x - 9*x^2).
 * https://oeis.org/A015545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(0) = 0, a(1) = 1,
 *           a(n) = 5*a(n-1) + 9*a(n-2)   for n >= 2
 *
 * This function computes the n-th term of OEIS sequence
 * A015545 (expansion of x/(1 - 5*x - 9*x^2)) using a
 * simple iterative memoised recurrence.  It works with
 * any seq_t type that supports the standard arithmetic
 * operators.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        /* allocate space for the first two terms */
        cache = (seq_t *)calloc(2, sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure – return zero */
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_len) return cache[n];

    /* otherwise expand the cache to n+1 */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (new_cache == NULL) {
        /* memory failure – fall back to returning zero */
        return 0;
    }
    cache = new_cache;

    /* compute newly required terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = 5 * cache[i - 1] + 9 * cache[i - 2];
    }
    cache_len = new_len;
    return cache[n];
}
