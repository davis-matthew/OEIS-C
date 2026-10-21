/*
 * A054477  A Pellian-related sequence.
 * https://oeis.org/A054477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - a(n-2); a(0) = 1, a(1) = 13
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute A054477: a(n) = 5*a(n-1) - a(n-2), a(0)=1, a(1)=13.  */
seq_t a(idx_t n)
{
    /* static cache to store computed values; grows as needed. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure cache exists and has room for indices 0 and 1 */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure: return 0 as fallback */
        cache[0] = 1;                  /* a(0) */
        cache[1] = 13;                 /* a(1) */
    }

    /* If n is already computed, return it */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Expand cache to hold up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;               /* allocation failure: return 0 */
    cache = tmp;
    cache_len = new_len;

    /* Compute values from the current length up to n */
    for (size_t i = 2; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)5 * cache[i - 1] - cache[i - 2];
    }

    return cache[n];
}
