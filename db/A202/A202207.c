/*
 * A202207  a(n) = 3*a(n-1) - a(n-2) + a(n-3) with a(0)=1, a(1)=2, a(2)=5.
 * https://oeis.org/A202207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + a(n-3) with a(0)=1, a(1)=2, a(2)=5
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache to make calls in any order efficient. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of elements allocated */

    /* If the value is already computed, return it immediately. */
    if (n < cache_size) {
        return cache[n];
    }

    /* Allocate or grow the cache to hold values up to n. */
    idx_t needed = n + 1;
    if (needed > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) {          /* allocation failed – abort by returning 0 */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_size = needed;
    }

    /* Compute any entries that are not yet in the cache. */
    idx_t start = (cache_size == 0) ? 0 : cache_size - (cache_size < 3 ? (idx_t)1 : (idx_t)1); /* safe start */
    if (start > cache_size) start = cache_size;  /* just in case */
    for (idx_t i = start; i < needed; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)2;
        } else if (i == 2) {
            cache[i] = (seq_t)5;
        } else {
            /* Recurrence: a(n) = 3*a(n-1) - a(n-2) + a(n-3) */
            cache[i] = (seq_t)3 * cache[i-1] - cache[i-2] + cache[i-3];
        }
    }

    return cache[n];
}
