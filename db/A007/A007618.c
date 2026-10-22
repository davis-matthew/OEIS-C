/*
 * A007618  a(n) = a(n-1) + sum of digits of a(n-1), a(1) = 5.
 * https://oeis.org/A007618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + sum of digits of a(n-1), a(1) = 5
 * Range:     u8 n <= 28, u16 n <= 3390, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t digit_sum(seq_t x) {
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (n > cache_len) {
        idx_t new_len = (n > 1000) ? n + 1000 : 2000;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        
        if (cache != NULL) {
            for (idx_t i = 0; i < computed_len; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        
        cache = new_cache;
        cache_len = new_len;
    }
    
    if (computed_len == 0) {
        cache[0] = 5;
        computed_len = 1;
    }
    
    while (computed_len < n) {
        cache[computed_len] = cache[computed_len - 1] + digit_sum(cache[computed_len - 1]);
        computed_len++;
    }
    
    return cache[n - 1];
}
