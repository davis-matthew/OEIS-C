/*
 * A003765  Number of Hamiltonian cycles in W_4 X P_n.
 * https://oeis.org/A003765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 3*a(n-2) - 19*a(n-3) + 20*a(n-4) - 4*a(n-5),
 *            a(1) = 1, a(2) = 10, a(3) = 46, a(4) = 238, a(5) = 1170, a(6) =
 *            5882
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        cache_len = 10;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        
        cache[1] = 1;
        cache[2] = 10;
        cache[3] = 46;
        cache[4] = 238;
        cache[5] = 1170;
        cache[6] = 5882;
        
        for (idx_t i = 7; i < cache_len; i++) {
            cache[i] = 5 * cache[i-1] + 3 * cache[i-2] - 19 * cache[i-3] + 20 * cache[i-4] - 4 * cache[i-5];
        }
    }
    
    while (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len *= 2;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        
        for (idx_t i = old_len; i < cache_len; i++) {
            cache[i] = 5 * cache[i-1] + 3 * cache[i-2] - 19 * cache[i-3] + 20 * cache[i-4] - 4 * cache[i-5];
        }
    }
    
    return cache[n];
}
