/*
 * A005711  a(n) = a(n-1) + a(n-9) for n >= 9; a(n) = 1 for n=0..7; a(8) = 2.
 * https://oeis.org/A005711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-9) for n >= 9; a(n) = 1 for n=0..7; a(8) = 2
 * Range:     u8 n <= 32, u16 n <= 60, u32 n <= 118, u64 n <= 233,
 *            u128 n <= 462, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t computed_up_to = 0;
    static bool first_call = true;
    
    if (first_call) {
        cache_len = 9;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i <= 7; i++) {
            cache[i] = 1;
        }
        cache[8] = 2;
        computed_up_to = 8;
        first_call = false;
    }
    
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        cache[i] = cache[i - 1] + cache[i - 9];
    }
    
    if (n > computed_up_to) {
        computed_up_to = n;
    }
    
    return cache[n];
}
