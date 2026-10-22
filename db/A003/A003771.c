/*
 * A003771  Number of Hamiltonian cycles in K_4 X P_n.
 * https://oeis.org/A003771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 2*a(n-2), n > 3; a(1) = 3, a(2) = 30, a(3) =
 *            198
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t max_computed = 3;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)30;
    if (n == 3) return (seq_t)198;
    
    // Initialize cache if needed
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)30;
        cache[3] = (seq_t)198;
    }
    
    // Resize cache if needed
    while (n >= cache_len) {
        idx_t new_len = cache_len * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = new_cache;
        for (idx_t i = cache_len; i < new_len; i++) {
            cache[i] = 0;
        }
        cache_len = new_len;
    }
    
    // Compute up to n
    for (idx_t i = max_computed + 1; i <= n; i++) {
        cache[i] = (seq_t)7 * cache[i-1] - (seq_t)2 * cache[i-2];
    }
    max_computed = (n > max_computed) ? n : max_computed;
    
    return cache[n];
}
