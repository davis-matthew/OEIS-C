/*
 * A003685  Number of Hamiltonian paths in P_3 X P_n.
 * https://oeis.org/A003685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - 12*a(n-3) + 4*a(n-4) + 12*a(n-5) -
 *            8*a(n-6) for n > 8. Base: a(1)=1, a(2)=8, a(3)=20, a(4)=62,
 *            a(5)=132, a(6)=336, a(7)=688, a(8)=1578
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t base[9] = {0, 1, 8, 20, 62, 132, 336, 688, 1578};
static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < 9) {
        return base[n];
    }
    
    if (cache == NULL) {
        cache_len = n + 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;
        for (idx_t i = 1; i < 9; i++) {
            cache[i] = base[i];
        }
        for (idx_t i = 9; i < cache_len; i++) {
            cache[i] = 3 * cache[i-1] + 2 * cache[i-2] - 12 * cache[i-3] 
                     + 4 * cache[i-4] + 12 * cache[i-5] - 8 * cache[i-6];
        }
    } else if (n >= cache_len) {
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (new_cache == NULL) return 0;
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        for (idx_t i = cache_len; i <= n; i++) {
            new_cache[i] = 3 * new_cache[i-1] + 2 * new_cache[i-2] - 12 * new_cache[i-3] 
                         + 4 * new_cache[i-4] + 12 * new_cache[i-5] - 8 * new_cache[i-6];
        }
        free(cache);
        cache = new_cache;
        cache_len = n + 1;
    }
    
    return cache[n];
}
