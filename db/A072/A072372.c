/*
 * A072372  a(0) = 1, a(1) = 1, a(n) = 2*a(n-1) + (2*n-1)^2*a(n-2) for n > 1.
 * https://oeis.org/A072372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(n)=2*a(n-1)+(2*n-1)^2*a(n-2) (n>1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  max_n      = 1;  /* a(0) and a(1) are known after init */

    /* First call: allocate base cache and initialise a(0) and a(1) */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* Grow cache if requested index is beyond current capacity */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return 0;           /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* If we already have the value, return it */
    if (n <= max_n) return cache[n];

    /* Compute missing terms up to n (strictly increasing order) */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t two_i_minus_one = (seq_t)2 * (seq_t)i - (seq_t)1;  /* 2*i-1 */
        seq_t square = two_i_minus_one * two_i_minus_one;
        seq_t term = cache[i - 1] * (seq_t)2;
        term += cache[i - 2] * square;
        cache[i] = term;
    }

    max_n = n;
    return cache[n];
}
