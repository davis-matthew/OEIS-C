/*
 * A005650  Number of "magic squares" of order n (see comment line for exact
 *          definition).
 * https://oeis.org/A005650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (2*n^2 - 4*n + 1)*a(n-2) - (n-3)*n*a(n-4), a(0) = 1, a(1)
 *            = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    
    // Ensure cache is large enough
    if (cache == NULL || cache_len <= n) {
        idx_t new_len = n + 1;
        if (new_len < 64) new_len = 64;
        
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (new_cache == NULL) return 0;
        
        memset(new_cache, 0, new_len * sizeof(seq_t));
        
        if (cache != NULL) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        
        cache = new_cache;
        cache_len = new_len;
    }
    
    // Initialize base cases
    cache[0] = 1;
    cache[1] = 1;
    
    // Compute from 2 up to n
    for (idx_t i = 2; i <= n; i++) {
        if (cache[i] == 0) { // Not computed yet
            if (i == 2) {
                cache[i] = 3;
            } else if (i == 3) {
                cache[i] = 7;
            } else {
                seq_t n_s = (seq_t)i;
                seq_t coeff1 = 2 * n_s * n_s - 4 * n_s + 1;
                seq_t coeff2 = (n_s - 3) * n_s;
                cache[i] = coeff1 * cache[i - 2] - coeff2 * cache[i - 4];
            }
        }
    }
    
    return cache[n];
}
