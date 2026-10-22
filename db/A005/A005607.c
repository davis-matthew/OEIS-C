/*
 * A005607  a(n) = (a(n-1) + a(n-2))!.
 * https://oeis.org/A005607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-1) + a(n-2))!, a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t factorial(seq_t n) {
    seq_t result = 1;
    for (seq_t i = 2; i <= n; i++) {
        result = result * i;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 1;
    
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    
    if (cache_size <= n) {
        idx_t new_size = n + 100;
        seq_t *new_cache = (seq_t *)malloc((new_size) * sizeof(seq_t));
        
        if (cache == NULL) {
            new_cache[0] = 1;
            new_cache[1] = 1;
            for (idx_t i = 2; i < new_size; i++) {
                new_cache[i] = 0;
            }
        } else {
            for (idx_t i = 0; i < cache_size; i++) {
                new_cache[i] = cache[i];
            }
            for (idx_t i = cache_size; i < new_size; i++) {
                new_cache[i] = 0;
            }
            free(cache);
        }
        
        cache = new_cache;
        cache_size = new_size;
    }
    
    for (idx_t i = 2; i <= n; i++) {
        if (cache[i] == 0) {
            seq_t sum = cache[i-1] + cache[i-2];
            cache[i] = factorial(sum);
        }
    }
    
    return cache[n];
}
