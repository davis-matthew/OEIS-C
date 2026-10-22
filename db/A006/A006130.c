/*
 * A006130  a(n) = a(n-1) + 3*a(n-2) for n > 1, a(0) = a(1) = 1.
 * https://oeis.org/A006130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    
    if (cache_size <= n) {
        idx_t new_size = (n + 1) * 2;
        if (new_size < 1024) new_size = 1024;
        
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (new_cache == NULL) {
            return 0;
        }
        
        if (cache != NULL) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        
        cache = new_cache;
        cache_size = new_size;
    }
    
    if (computed_up_to < 2) {
        cache[0] = 1;
        cache[1] = 1;
        computed_up_to = 2;
    }
    
    for (idx_t i = computed_up_to; i <= n; i++) {
        cache[i] = cache[i-1] + (seq_t)3 * cache[i-2];
    }
    
    if (n >= computed_up_to) {
        computed_up_to = n + 1;
    }
    
    return cache[n];
}
