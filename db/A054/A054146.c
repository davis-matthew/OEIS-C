/*
 * A054146  a(n) = A054145(n)/2.
 * https://oeis.org/A054146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=6, a(3)=29,
   a(n)=8*a(n-1)-20*a(n-2)+16*a(n-3)-4*a(n-4) for n≥4 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* initialise the cache with the first four terms */
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) abort();
#ifdef __cplusplus
        for (idx_t i = 0; i < cache_len; ++i) new (&cache[i]) seq_t();
#endif
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 6;
        cache[3] = 29;
    }

    if (n < cache_len) {
        return cache[n];
    }

    /* extend the cache up to n (inclusive) */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!tmp) abort();
    cache = tmp;
#ifdef __cplusplus
    for (idx_t i = cache_len; i < new_len; ++i) new (&cache[i]) seq_t();
#endif
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = 8 * cache[i-1]
                 - 20 * cache[i-2]
                 + 16 * cache[i-3]
                 - 4  * cache[i-4];
    }
    cache_len = new_len;
    return cache[n];
}
