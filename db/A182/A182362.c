/*
 * A182362  a(n+1) = a(n) + floor(a(n)/9) with a(0)=9.
 * https://oeis.org/A182362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+1) = a(n) + floor(a(n)/9), a(0) = 9 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t  cache_len = 0;   /* number of values already computed */
    static size_t  cache_cap = 0;   /* capacity of cache */

    /* enlarge cache if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first element if cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)9;
        cache_len = 1;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = prev + prev / (seq_t)9;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
