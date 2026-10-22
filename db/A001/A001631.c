/*
 * A001631  Tetranacci numbers: a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4), with
 *          initial conditions a(0..3) = (0, 0, 1, 0).
 * https://oeis.org/A001631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4), with a(0)=0, a(1)=0,
 *            a(2)=1, a(3)=0
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 37, u64 n <= 71, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_capacity = 0;
static idx_t cache_computed = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 0;
    
    if (cache == NULL || n >= cache_capacity) {
        idx_t new_cap = (n < 1024) ? 1024 : (n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_computed; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        
        cache = new_cache;
        cache_capacity = new_cap;
    }
    
    if (cache_computed < 4) {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 0;
        cache_computed = 4;
    }
    
    while (cache_computed <= n) {
        idx_t i = cache_computed;
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4];
        cache_computed++;
    }
    
    return cache[n];
}
