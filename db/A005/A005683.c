/*
 * A005683  Numbers of Twopins positions.
 * https://oeis.org/A005683
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-4) - a(n-6), a(3)=1, a(4)=2, a(5)=3,
 *            a(6)=5, a(7)=8, a(8)=13
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t next_idx = 9;
    
    if (cache == NULL) {
        cache_len = 16;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[3] = 1;
        cache[4] = 2;
        cache[5] = 3;
        cache[6] = 5;
        cache[7] = 8;
        cache[8] = 13;
    }
    
    if (n <= 8) {
        return cache[n];
    }
    
    while (n >= cache_len) {
        idx_t new_len = cache_len * 2;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    while (next_idx <= n) {
        cache[next_idx] = 2 * cache[next_idx-1] - cache[next_idx-4] - cache[next_idx-6];
        next_idx++;
    }
    
    return cache[n];
}
