/*
 * A099214  a(n)=4a(n-1)-4a(n-2)+4a(n-3).
 * https://oeis.org/A099214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 4*a(n-2) + 4*a(n-3).  
   Initial values: a(0)=1, a(1)=2, a(2)=4.  */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n < cache_len)
        return cache[n];

    /* Initialise the first three terms once. */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache_len = 3;
    }

    /* Grow storage and compute missing terms. */
    if (n + 1 > cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;

        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_len = new_len;

        for (idx_t i = old_len; i <= n; ++i) {
            seq_t t = (seq_t)4 * cache[i-1];
            t -= (seq_t)4 * cache[i-2];
            t += (seq_t)4 * cache[i-3];
            cache[i] = t;
        }
    }

    return cache[n];
}
