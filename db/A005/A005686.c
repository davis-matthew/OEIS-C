/*
 * A005686  Number of Twopins positions.
 * https://oeis.org/A005686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-5), a(0)=0, a(1)=1, a(2)=1, a(3)=1, a(4)=1
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 107, u64 n <= 212,
 *            u128 n <= 421, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_len = 100;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        memset(cache, 0, cache_len * sizeof(seq_t));
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 1;
        for (idx_t i = 5; i < cache_len; i++) {
            cache[i] = cache[i - 2] + cache[i - 5];
        }
    }
    
    if (n >= cache_len) {
        idx_t new_len = cache_len;
        while (new_len <= n) {
            new_len *= 2;
        }
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        for (idx_t i = cache_len; i < new_len; i++) {
            new_cache[i] = new_cache[i - 2] + new_cache[i - 5];
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    return cache[n];
}
