/*
 * A006937  Convert the last term from decimal to binary! a(1)=10.
 * https://oeis.org/A006937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = convert a(n-1) to binary, then interpret binary digits as
 *            decimal; a(1) = 10
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    
    if (n == 1) return 10;
    
    if (cache == NULL) {
        cache_size = (n > 1000) ? n + 1000 : 2000;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 10;
        for (idx_t i = 1; i < cache_size; i++) {
            cache[i] = 0;
        }
    } else if (n > cache_size) {
        seq_t *old_cache = cache;
        idx_t old_size = cache_size;
        cache_size = n + 1000;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        
        for (idx_t i = 0; i < old_size; i++) {
            cache[i] = old_cache[i];
        }
        
        for (idx_t i = old_size; i < cache_size; i++) {
            cache[i] = 0;
        }
        
        free(old_cache);
    }
    
    for (idx_t i = 1; i < n; i++) {
        if (cache[i] == 0) {
            seq_t prev = cache[i - 1];
            seq_t bits = 0;
            seq_t temp = prev;
            seq_t power = 1;
            
            while (temp > 0) {
                seq_t bit = temp % 2;
                bits = bits + bit * power;
                power = power * 10;
                temp = temp / 2;
            }
            
            cache[i] = bits;
        }
    }
    
    return cache[n - 1];
}
