/*
 * A007574  Patterns in a dual ring.
 * https://oeis.org/A007574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 4*a(n-4) - a(n-6), with
 *            a(1)=1, a(2)=3, a(3)=7, a(4)=15, a(5)=31, a(6)=60
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 39, u64 n <= 84, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

seq_t a(idx_t n) {
    static seq_t base[6] = {1, 3, 7, 15, 31, 60};
    
    if (n <= 6) return base[n - 1];
    
    if (cache == NULL) {
        cache_cap = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 6;
        for (idx_t i = 0; i < 6; i++) {
            cache[i] = base[i];
        }
    }
    
    if (n > cache_len) {
        if (n > cache_cap) {
            while (cache_cap < n) cache_cap *= 2;
            seq_t *new_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
            cache = new_cache;
        }
        
        for (idx_t i = cache_len + 1; i <= n; i++) {
            idx_t idx = i - 1;
            cache[idx] = 4 * cache[idx - 1] - 4 * cache[idx - 2] - 2 * cache[idx - 3] 
                       + 4 * cache[idx - 4] - cache[idx - 6];
        }
        
        cache_len = n;
    }
    
    return cache[n - 1];
}
