/*
 * A003758  Number of 2-factors in D_4 X P_n.
 * https://oeis.org/A003758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 9*a(n-2) - 3*a(n-3) - 3*a(n-4) + a(n-5), n>5;
 *            a(1) = 0, a(2) = 3, a(3) = 7, a(4) = 46, a(5) = 193
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    seq_t init[] = {0, 0, 3, 7, 46, 193};
    if (n <= 5) return init[n];
    
    if (cache == NULL) {
        cache_len = 256;
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        for (idx_t i = 0; i < 6; i++) {
            cache[i] = init[i];
        }
        max_computed = 5;
    }
    
    while (n >= cache_len) {
        idx_t new_len = cache_len * 2;
        seq_t *new_cache = (seq_t *)malloc((new_len + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= max_computed; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; i++) {
            cache[i] = 3 * cache[i-1] + 9 * cache[i-2] - 3 * cache[i-3] - 3 * cache[i-4] + cache[i-5];
        }
        max_computed = n;
    }
    
    return cache[n];
}
