/*
 * A007380  Number of 5th-order maximal independent sets in path graph.
 * https://oeis.org/A007380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-7), a(1)=1, a(2)=2, a(3)=1, a(4)=3, a(5)=1,
 *            a(6)=4, a(7)=2
 * Range:     u8 n <= 31, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    static const seq_t base[] = {1, 2, 1, 3, 1, 4, 2};
    
    if (n <= 7) {
        return (n == 0) ? 0 : base[n - 1];
    }
    
    if (cache_len < n) {
        idx_t new_len = (n < 10000) ? 10000 : n + 1;
        seq_t *new_cache = mem_new(seq_t, new_len + 1);
        
        idx_t start = 8;
        if (cache != NULL) {
            mem_copy(new_cache, cache, sizeof(seq_t) * (cache_len + 1));
            start = cache_len + 1;
            mem_free(cache);
        }
        
        for (idx_t i = 1; i <= 7; i++) {
            new_cache[i] = base[i - 1];
        }
        
        for (idx_t i = start; i <= new_len; i++) {
            new_cache[i] = new_cache[i - 2] + new_cache[i - 7];
        }
        
        cache = new_cache;
        cache_len = new_len;
    }
    
    return cache[n];
}
