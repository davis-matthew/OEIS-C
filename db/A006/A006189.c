/*
 * A006189  Number of self-avoiding walks of any length from NW to SW corners
 *          of a grid or lattice with n rows and 3 columns.
 * https://oeis.org/A006189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + 2*a(n-3) + a(n-4), a(0)=1, a(1)=1,
 *            a(2)=3, a(3)=11
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 11;
    
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 11;
        
        for (idx_t i = 4; i <= n; i++) {
            cache[i] = 4 * cache[i-1] - 3 * cache[i-2] + 2 * cache[i-3] + cache[i-4];
        }
        cache_len = n + 1;
    } else if (n >= cache_len) {
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        
        for (idx_t i = cache_len; i <= n; i++) {
            new_cache[i] = 4 * new_cache[i-1] - 3 * new_cache[i-2] + 2 * new_cache[i-3] + new_cache[i-4];
        }
        
        free(cache);
        cache = new_cache;
        cache_len = n + 1;
    }
    
    return cache[n];
}
