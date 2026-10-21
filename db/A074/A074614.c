/*
 * A074614  a(n) = 4^n + 9^n.
 * https://oeis.org/A074614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 9^n Recurrence used: a(0)=2, a(1)=13,
 *            a(n)=13*a(n-1) - 36*a(n-2) for n ≥ 2 Cache results in a dynamic
 *            array so that calls may arrive in any order and each index is
 *            computed only once. The routine is compatible with both C11 and
 *            C++17 and works for any seq_t that supports arithmetic w...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size   = 0;   /* number of elements stored (cache_size == cache_size) */
    static idx_t cache_capacity = 0; /* allocated capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_capacity = 4;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)13;
        cache_size = 2;
    }

    /* if the requested index is already known, return it */
    if (n < cache_size) return cache[n];

    /* resize the cache if necessary */
    if (n >= cache_capacity) {
        idx_t new_cap = cache_capacity;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* recurrence: a(i) = 13*a(i-1) - 36*a(i-2) */
        cache[i] = (seq_t)13 * cache[i - 1] - (seq_t)36 * cache[i - 2];
    }
    cache_size = n + 1;

    return cache[n];
}
