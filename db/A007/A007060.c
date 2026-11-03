/*
 * A007060  Number of ways n married couples can sit in a row without any
 *          spouses next to each other.
 * https://oeis.org/A007060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*n*((2*n-1)*a(n-1) + (2*n-2)*a(n-2)), n > 1, a(0) = 1,
 *            a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 99 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    
    if (n >= cache_len) {
        seq_t *new_cache = mem_new(seq_t, (n + 1));
        
        if (cache) {
            for (idx_t i = 0; i < cache_len; i++) {
                new_cache[i] = cache[i];
            }
            mem_free(cache);
        } else {
            new_cache[0] = 1;
            new_cache[1] = 0;
        }
        
        cache = new_cache;
        idx_t start = cache_len > 0 ? cache_len : 2;
        
        for (idx_t i = start; i <= n; i++) {
            seq_t ni = (seq_t)i;
            seq_t coeff = ni * 2;
            cache[i] = coeff * ((ni * 2 - 1) * cache[i-1] + (ni * 2 - 2) * cache[i-2]);
        }
        
        cache_len = n + 1;
    }
    
    return cache[n];
}
