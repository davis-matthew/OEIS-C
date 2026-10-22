/*
 * A007982  Number of non-Abelian metacyclic groups of order 2^n.
 * https://oeis.org/A007982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) - 2*a(n-4) - a(n-5) + 2*a(n-6)
 *            + a(n-7) - a(n-8) for n >= 12
 * Range:     u8 n <= 18, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[] = {0, 0, 2, 5, 9, 15, 22, 32, 43, 57, 72, 91};
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    
    if (n <= 11) {
        return init[n - 1];
    }
    
    if (cache == NULL) {
        cache_size = 12;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 1; i <= 11; i++) {
            cache[i] = init[i - 1];
        }
        cache[0] = 0;
    }
    
    if (n >= cache_size) {
        idx_t new_size = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        
        for (idx_t i = 0; i < cache_size; i++) {
            new_cache[i] = cache[i];
        }
        
        for (idx_t k = cache_size; k < new_size; k++) {
            new_cache[k] = new_cache[k-1] + 2*new_cache[k-2] - new_cache[k-3] - 2*new_cache[k-4] - new_cache[k-5] + 2*new_cache[k-6] + new_cache[k-7] - new_cache[k-8];
        }
        
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }
    
    return cache[n];
}
