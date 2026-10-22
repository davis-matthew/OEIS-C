/*
 * A000804  Permanent of a certain cyclic n X n (0,1) matrix.
 * https://oeis.org/A000804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) from recurrence derived from generating function: a(n) =
 *            3*a(n-1) - 2*a(n-3) - 2*a(n-4) - 6*a(n-5) + 2*a(n-6) + 4*a(n-7)
 *            + 2*a(n-8) + 2*a(n-9) - a(n-10) - a(n-11) - p(n) where p(n) is
 *            coefficient of x^n in numerator polynomial, 0 for n > 15
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static const seq_t init[] = {
        1, 1, 2, 6, 24, 120, 265, 579, 1265, 2783, 6208
    };
    
    if (n < 11) {
        return init[n];
    }
    
    static const long long num[] = {
        -1, 2, 1, -2, -10, -60, 75, 150, 158, 122, -190, -213, -113, -48, 64, 41
    };
    
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t computed_up_to = 10;
    
    idx_t needed = n + 1;
    if (cache == NULL) {
        cache_len = (needed < 2048) ? 2048 : (needed * 2);
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < 11; i++) {
            cache[i] = init[i];
        }
    } else if (needed > cache_len) {
        idx_t new_len = cache_len;
        while (new_len < needed) {
            new_len *= 2;
        }
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t result = (seq_t)3 * cache[i-1];
        result -= (seq_t)2 * cache[i-3];
        result -= (seq_t)2 * cache[i-4];
        result -= (seq_t)6 * cache[i-5];
        result += (seq_t)2 * cache[i-6];
        result += (seq_t)4 * cache[i-7];
        result += (seq_t)2 * cache[i-8];
        result += (seq_t)2 * cache[i-9];
        result -= cache[i-10];
        result -= cache[i-11];
        
        if (i < 16) {
            result -= (seq_t)num[i];
        }
        
        cache[i] = result;
    }
    
    computed_up_to = (n > computed_up_to) ? n : computed_up_to;
    
    return cache[n];
}
