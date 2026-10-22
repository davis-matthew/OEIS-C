/*
 * A005257  Number of weighted voting procedures.
 * https://oeis.org/A005257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1)=2, a(2)=3, a(3)=5, for n>3: a(n) = 3*a(n-1) - 2*a(n-2) if n
 *            is even a(n) = a(n-1) + 2*a(n-2) - a(floor((n-1)/2) - 1) if n is
 *            odd
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static bool initialized = false;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (n <= 3) {
        if (n == 1) return 2;
        if (n == 2) return 3;
        return 5;
    }
    
    if (cache == NULL || cache_len < n) {
        idx_t new_len = (cache_len == 0) ? n : cache_len;
        while (new_len < n) new_len *= 2;
        
        seq_t *new_cache = (seq_t *)calloc(new_len + 1, sizeof(seq_t));
        if (new_cache == NULL) return 0;
        
        if (cache != NULL) {
            for (idx_t i = 1; i <= cache_len; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        
        cache = new_cache;
        cache_len = new_len;
        initialized = false;
    }
    
    if (!initialized) {
        cache[1] = 2;
        cache[2] = 3;
        cache[3] = 5;
        initialized = true;
    }
    
    for (idx_t i = 4; i <= n; i++) {
        if (cache[i] == 0) {
            if (i % 2 == 0) {
                cache[i] = 3 * cache[i-1] - 2 * cache[i-2];
            } else {
                idx_t k = ((i - 1) / 2) - 1;
                cache[i] = cache[i-1] + 2 * cache[i-2] - cache[k];
            }
        }
    }
    
    return cache[n];
}
