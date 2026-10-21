/*
 * A080424  a(n) = 3*a(n-1) + 18*a(n-2), a(0)=0, a(1)=1.
 * https://oeis.org/A080424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 18*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache_len == 0) {                     /* initialise cache with a(0), a(1) */
        size_t init = 2;
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    }

    if (n + 1 > cache_len) {                  /* extend cache to at least n+1 entries */
        size_t old_len = cache_len;
        size_t new_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = old_len; i <= n; ++i) {
            cache[i] = (seq_t)3 * cache[i-1] + (seq_t)18 * cache[i-2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
