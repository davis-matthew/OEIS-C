/*
 * A000971  Fermat coefficients.
 * https://oeis.org/A000971
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_{n-6} + 6*a(n-1) - 15*a(n-2) + 19*a(n-3) - 9*a(n-4) -
 *            9*a(n-5) + 18*a(n-6) - 9*a(n-7) - 9*a(n-8) + 19*a(n-9) -
 *            15*a(n-10) + 6*a(n-11) - a(n-12), with p =
 *            [1,3,3,-4,10,-4,3,3,1] for n in [6,14]
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 161, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_capacity = 0;
static idx_t cache_filled = 0;

seq_t a(idx_t n) {
    if (n < 6) return 0;
    
    if (n >= cache_capacity) {
        idx_t new_capacity = n + 1024;
        cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        cache_capacity = new_capacity;
    }
    
    if (cache_filled == 0) {
        for (idx_t i = 0; i < 6; i++) cache[i] = 0;
        cache_filled = 6;
    }
    
    static int p[] = {1, 3, 3, -4, 10, -4, 3, 3, 1};
    
    for (idx_t i = cache_filled; i <= n; i++) {
        seq_t p_val = (i >= 6 && i <= 14) ? (seq_t)p[i - 6] : 0;
        seq_t term = p_val + 6 * cache[i - 1] - 15 * cache[i - 2] + 19 * cache[i - 3]
                     - 9 * cache[i - 4] - 9 * cache[i - 5] + 18 * cache[i - 6];
        
        if (i >= 7) term -= 9 * cache[i - 7];
        if (i >= 8) term -= 9 * cache[i - 8];
        if (i >= 9) term += 19 * cache[i - 9];
        if (i >= 10) term -= 15 * cache[i - 10];
        if (i >= 11) term += 6 * cache[i - 11];
        if (i >= 12) term -= cache[i - 12];
        
        cache[i] = term;
    }
    
    if (n + 1 > cache_filled) cache_filled = n + 1;
    
    return cache[n];
}
