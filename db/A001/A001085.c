/*
 * A001085  a(n) = 20*a(n-1) - a(n-2).
 * https://oeis.org/A001085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - a(n-2), a(0) = 1, a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 10;
    
    // Ensure cache capacity
    if (n >= cache_cap) {
        idx_t new_cap = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_len; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        
        cache = new_cache;
        cache_cap = new_cap;
    }
    
    // Initialize base cases if needed
    if (cache_len == 0) {
        cache[0] = 1;
        cache[1] = 10;
        cache_len = 2;
    }
    
    // Compute values up to n
    while (cache_len <= n) {
        idx_t i = cache_len;
        cache[i] = (seq_t)20 * cache[i-1] - cache[i-2];
        cache_len++;
    }
    
    return cache[n];
}
