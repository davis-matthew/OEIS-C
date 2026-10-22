/*
 * A006864  Number of Hamiltonian cycles in P_4 X P_n.
 * https://oeis.org/A006864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 2*a(n-3) + a(n-4), a(1) = 0, a(2) =
 *            1, a(3) = 2, a(4) = 6
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 2;
    if (n == 4) return 6;
    
    // Ensure cache is large enough
    if (n > cache_len) {
        idx_t new_len = (cache_len == 0) ? 16 : cache_len;
        while (new_len < n) {
            new_len *= 2;
        }
        
        if (cache == NULL) {
            cache = (seq_t *)calloc(new_len + 1, sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        }
        cache_len = new_len;
    }
    
    // Initialize base cases in cache if not done yet
    if (computed_up_to < 4) {
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 2;
        cache[4] = 6;
        computed_up_to = 4;
    }
    
    // Compute sequentially from computed_up_to+1 to n
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        cache[i] = 2*cache[i-1] + 2*cache[i-2] - 2*cache[i-3] + cache[i-4];
    }
    
    computed_up_to = n;
    
    return cache[n];
}
