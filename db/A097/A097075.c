/*
 * A097075  Expansion of g.f. (1-x-x^2)/(1-x-3*x^2-x^3).
 * https://oeis.org/A097075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + a(n-3), a(0) = 1, a(1) = 0, a(2) = 2
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache    = NULL;
    static idx_t cache_len = 0;   /* number of valid entries in cache */
    static idx_t cache_cap = 0;   /* allocated size of cache */

    /* initialise cache with first three terms if empty */
    if (cache_len == 0) {
        cache_cap = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;          /* allocation failure: return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache_len = 3;
    }

    /* if requested index already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* grow allocation if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;   /* double until enough */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) return (seq_t)0;      /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* safe arithmetic: all operands are seq_t, the result fits in seq_t mod arith */
        cache[i] = cache[i-1] + (seq_t)3 * cache[i-2] + cache[i-3];
    }

    cache_len = n + 1;
    return cache[n];
}
