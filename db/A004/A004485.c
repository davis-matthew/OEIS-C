/*
 * A004485  Sprague-Grundy values for game of Wyt Queens.
 * https://oeis.org/A004485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-12) - a(n-13) for n > 21, with base cases
 *            a(0)..a(21)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t base[] = {
        4, 5, 3, 2, 7, 6, 9, 0, 1, 8, 13, 12,
        11, 16, 15, 10, 19, 18, 17, 14, 21, 20
    };
    
    if (n <= 21) {
        return base[n];
    }
    
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        cache_len = 22;
        cache = (seq_t *)malloc(22 * sizeof(seq_t));
        for (idx_t i = 0; i < 22; i++) {
            cache[i] = base[i];
        }
    }
    
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        
        for (idx_t i = old_len; i < cache_len; i++) {
            cache[i] = cache[i-1] + cache[i-12] - cache[i-13];
        }
    }
    
    return cache[n];
}
