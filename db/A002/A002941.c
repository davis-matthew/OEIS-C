/*
 * A002941  Arrays of dumbbells.
 * https://oeis.org/A002941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) - 2*a(n-3) + 10*a(n-4) - 2*a(n-5) -
 *            5*a(n-6) + a(n-7) + a(n-8) for n >= 9
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 30, u64 n <= 72, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static const seq_t base_vals[] = {1, 7, 29, 94, 263, 667, 1577, 3538};
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;
    
    if (n < 1) return 0;
    if (n <= 8) return base_vals[n - 1];
    
    if (cache == NULL) {
        cache_cap = 16;
        while (cache_cap < n) cache_cap *= 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 8; i++) {
            cache[i] = base_vals[i];
        }
        cache_len = 8;
    }
    
    while (cache_cap < n) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    
    while (cache_len < n) {
        idx_t i = cache_len + 1;
        seq_t val = (seq_t)5 * cache[i - 2] - (seq_t)7 * cache[i - 3] 
                  - (seq_t)2 * cache[i - 4] + (seq_t)10 * cache[i - 5] 
                  - (seq_t)2 * cache[i - 6] - (seq_t)5 * cache[i - 7] 
                  + cache[i - 8] + cache[i - 9];
        cache[cache_len] = val;
        cache_len++;
    }
    
    return cache[n - 1];
}
