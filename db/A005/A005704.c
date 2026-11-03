/*
 * A005704  Number of partitions of 3n into powers of 3.
 * https://oeis.org/A005704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(floor(n/3)), a(0) = 1
 * Range:     u8 n <= 27, u16 n <= 160, u32 n <= 1691, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9325 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        cache_len = 1;
        cache = mem_new(seq_t, 1);
        cache[0] = 1;
    }
    
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = mem_resize(seq_t, cache, cache_len);
        
        for (idx_t i = old_len; i <= n; i++) {
            cache[i] = cache[i - 1] + cache[i / 3];
        }
    }
    
    return cache[n];
}
