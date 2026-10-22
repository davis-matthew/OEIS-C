/*
 * A001657  Fibonomial coefficients: column 5 of A010048.
 * https://oeis.org/A001657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 40*a(n-2) - 60*a(n-3) - 40*a(n-4) + 8*a(n-5) +
 *            a(n-6), a(0)=1, a(1)=8, a(2)=104, a(3)=1092, a(4)=12376,
 *            a(5)=136136
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
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
    
    if (cache == NULL) {
        cache_len = 1024;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        
        cache[0] = 1;
        cache[1] = 8;
        cache[2] = 104;
        cache[3] = 1092;
        cache[4] = 12376;
        cache[5] = 136136;
        
        for (idx_t i = 6; i < cache_len; i++) {
            cache[i] = (seq_t)8 * cache[i-1] + (seq_t)40 * cache[i-2]
                     - (seq_t)60 * cache[i-3] - (seq_t)40 * cache[i-4]
                     + (seq_t)8 * cache[i-5] + cache[i-6];
        }
    }
    
    if (n >= cache_len) {
        seq_t *old_cache = cache;
        idx_t old_len = cache_len;
        
        while (cache_len <= n) {
            cache_len *= 2;
        }
        
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        memcpy(cache, old_cache, sizeof(seq_t) * old_len);
        free(old_cache);
        
        for (idx_t i = old_len; i < cache_len; i++) {
            cache[i] = (seq_t)8 * cache[i-1] + (seq_t)40 * cache[i-2]
                     - (seq_t)60 * cache[i-3] - (seq_t)40 * cache[i-4]
                     + (seq_t)8 * cache[i-5] + cache[i-6];
        }
    }
    
    return cache[n];
}
