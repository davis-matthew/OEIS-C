/*
 * A001046  a(n) = n*(n-1)*a(n-1)/2 + a(n-2), a(0) = a(1) = 1.
 * https://oeis.org/A001046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*a(n-1)/2 + a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_cap = 100;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;
        cache[0] = 1;
        cache[1] = 1;
        cache_len = 2;
    }
    
    if (n < cache_len) {
        return cache[n];
    }
    
    while (cache_len <= n) {
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (new_cache == NULL) return 0;
            cache = new_cache;
        }
        
        idx_t i = cache_len;
        seq_t ni = (seq_t)i;
        seq_t coeff = ni * (ni - 1) / 2;
        cache[i] = coeff * cache[i - 1] + cache[i - 2];
        cache_len++;
    }
    
    return cache[n];
}
