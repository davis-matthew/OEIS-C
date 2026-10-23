/*
 * A015007  q-factorial numbers for q=8.
 * https://oeis.org/A015007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} ((8^k - 1)/(8-1)) Recurrence: a(0) = 1,
 *            a(n) = a(n-1)*(8^n-1)/7
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms to handle arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of stored terms (cache_size == 0 => none) */
    static seq_t pow8_cached = (seq_t)1;  /* 8^(cache_size-1), 8^0 after first allocation */

    /* initialize cache with a(0) if not yet done */
    if (cache_size == 0) {
        cache = (seq_t*) malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out of memory: return zero */
        cache[0] = (seq_t)1;
        cache_size = 1;
        pow8_cached = (seq_t)1;           /* 8^0 */
    }

    /* return cached term if already computed */
    if (n < cache_size)
        return cache[n];

    /* extend cache to hold up to n */
    size_t new_len = n + 1;
    seq_t *new_cache = (seq_t*) realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;      /* out of memory */
    cache = new_cache;

    /* compute new terms starting from current cache_size */
    for (idx_t i = cache_size; i <= n; ++i) {
        pow8_cached *= (seq_t)8;                          /* pow8_cached = 8^i */
        seq_t factor = (pow8_cached - (seq_t)1) / (seq_t)7;/* (8^i-1)/7 */
        cache[i] = cache[i-1] * factor;
    }

    cache_size = n + 1;                                 /* update cached size */
    return cache[n];
}
