/*
 * A174677  a(n) = 2*a(n-1)*a(n-2) with a(0)=1 and a(1)=1.
 * https://oeis.org/A174677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1)*a(n-2) with a(0)=1, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t  last_computed  = 0;

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_capacity = 2;
        if ((size_t)n + 1 > cache_capacity)
            cache_capacity = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last_computed = 1;
    } else if ((size_t)n + 1 > cache_capacity) {
        size_t new_capacity = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;
        cache = tmp;
        cache_capacity = new_capacity;
    }

    if (n <= last_computed)
        return cache[(size_t)n];

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t prod = cache[(size_t)i - 1] * cache[(size_t)i - 2];
        cache[(size_t)i] = prod + prod;
    }
    last_computed = n;
    return cache[(size_t)n];
}
