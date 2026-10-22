/*
 * A001128  Reverse digits of previous term and multiply by previous term.
 * https://oeis.org/A001128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reverse_digits(a(n-1)) * a(n-1), with a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_digits.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (n == 1) return 2;
    
    if (n < cache_len) {
        return cache[n - 1];
    }
    
    if (cache == NULL) {
        cache = (seq_t *)malloc(n * sizeof(seq_t));
        cache[0] = 2;
        
        for (idx_t i = 2; i <= n; i++) {
            seq_t reversed = reverse_digits(cache[i - 2]);
            cache[i - 1] = reversed * cache[i - 2];
        }
        
        cache_len = n;
    } else {
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        
        for (idx_t i = cache_len + 1; i <= n; i++) {
            seq_t reversed = reverse_digits(cache[i - 2]);
            cache[i - 1] = reversed * cache[i - 2];
        }
        
        cache_len = n;
    }
    
    return cache[n - 1];
}
