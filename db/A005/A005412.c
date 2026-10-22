/*
 * A005412  Number of non-vanishing Feynman diagrams of order 2n for the
 *          vacuum polarization (the proper two-point function of the photon)
 *          and for the self-energy (the proper two-point function of the
 *          electron) in quantum electrodynamics (QED).
 * https://oeis.org/A005412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n - 2) * a(n-1) + Sum_{k=1..n-1} a(k) * a(n-k), a(1) =
 *            1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
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
    if (n < 1) return 0;
    if (n == 1) return 1;
    
    // Extend cache if needed
    if (cache_len <= n) {
        idx_t new_len = cache_len ? cache_len * 2 : 2;
        while (new_len <= n) new_len *= 2;
        
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        memset(new_cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        cache = new_cache;
        cache_len = new_len;
    }
    
    // Ensure cache[1] is set
    if (cache[1] == 0) cache[1] = 1;
    
    // Compute all values from 2 to n
    for (idx_t i = 2; i <= n; i++) {
        if (cache[i] == 0) {
            seq_t result = ((seq_t)(i - 1)) * 2 * cache[i - 1];
            for (idx_t k = 1; k < i; k++) {
                result += cache[k] * cache[i - k];
            }
            cache[i] = result;
        }
    }
    
    return cache[n];
}
