/*
 * A001843  The coding-theoretic function A(n,4,4).
 * https://oeis.org/A001843
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-6) - a(n-7) - a(n-8) +
 *            a(n-9) + a(n-12) - a(n-13) - a(n-14) + a(n-15) - a(n-18) +
 *            a(n-19) + a(n-20) - a(n-21)
 * Range:     u8 n <= 19, u16 n <= 117, u32 n <= 1003, u64 n <= 1003,
 *            u128 n <= 1003, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


static seq_t *cache = NULL;
static bool *computed = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < 4) return 0;
    
    static const seq_t base_values[] = {
        1, 1, 3, 7, 14, 18, 30, 35, 51, 65, 91, 105, 140, 157, 198, 228, 285, 315, 385, 419, 498, 550, 650, 702
    };
    static const idx_t base_start = 4;
    static const idx_t base_end = 27;
    
    if (n <= base_end) {
        return base_values[n - base_start];
    }
    
    if (cache == NULL || n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = (n + 1) + 100;
        
        seq_t *new_cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (new_cache == NULL) return 0;
        cache = new_cache;
        
        bool *new_computed = (bool *)realloc(computed, cache_len * sizeof(bool));
        if (new_computed == NULL) return 0;
        computed = new_computed;
        
        for (idx_t i = old_len; i < cache_len; i++) {
            computed[i] = false;
        }
    }
    
    if (computed[n]) {
        return cache[n];
    }
    
    seq_t result = a(n-1) + a(n-2) - a(n-3) + a(n-6) - a(n-7) - a(n-8) + a(n-9) + a(n-12) - a(n-13) - a(n-14) + a(n-15) - a(n-18) + a(n-19) + a(n-20) - a(n-21);
    
    cache[n] = result;
    computed[n] = true;
    return result;
}
