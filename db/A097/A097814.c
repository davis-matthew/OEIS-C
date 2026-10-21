/*
 * A097814  Expansion of e.g.f. exp(3*x)/(1-3*x).
 * https://oeis.org/A097814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n*a(n-1) + 3^n, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoizes computed terms in a static dynamic array.
 * It works for any integer type chosen for seq_t and idx_t by the build
 * system.  The recurrence is evaluated iteratively from the last cached
 * index up to the requested n.  For the power of 3 a running product
 * pow3 is maintained across calls.  Memory is grown as needed. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries in cache */
    static idx_t cache_cap = 0;   /* allocated capacity of cache array */

    /* enlarge cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;       /* allocation failure, return 0 as a guard value */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* ensure first term is initialized */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* static value to hold 3^k where k = cache_len-1 */
    static seq_t pow3 = (seq_t)1;

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        pow3 *= (seq_t)3;                                  /* pow3 becomes 3^i */
        seq_t term = (seq_t)3 * (seq_t)i * cache[i-1] + pow3;
        cache[i] = term;
    }

    cache_len = n + 1;
    return cache[n];
}
