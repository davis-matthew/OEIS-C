/*
 * A004689  Fibonacci numbers written in base 6.
 * https://oeis.org/A004689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Fibonacci(n) written in base 6, interpreted as decimal
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 73, u128 n <= 145,
 *            big any n
 * Helpers:   convert_to_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/convert_to_base6.h"

static seq_t fib(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        
        if (cache != NULL) {
            for (idx_t i = 0; i < old_len; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
        } else {
            new_cache[0] = 0;
            new_cache[1] = 1;
            old_len = 2;
        }
        
        for (idx_t i = old_len; i <= n; i++) {
            new_cache[i] = new_cache[i - 1] + new_cache[i - 2];
        }
        
        cache = new_cache;
        cache_len = new_len;
    }
    
    return cache[n];
}

seq_t a(idx_t n) {
    return convert_to_base6(fib(n));
}
