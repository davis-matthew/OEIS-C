/*
 * A005025  Random walks.
 * https://oeis.org/A005025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 9*a(n-1) - 28*a(n-2) + 35*a(n-3) - 15*a(n-4) + a(n-5)
 *            Base cases: a(1)=9, a(2)=53, a(3)=260, a(4)=1156, a(5)=4845
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    const seq_t base[] = {0, 9, 53, 260, 1156, 4845};
    
    if (n <= 5) {
        return base[n];
    }
    
    if (cache == NULL) {
        cache_size = (n < 1000) ? 1000 : n + 100;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 6; i++) {
            cache[i] = base[i];
        }
        computed_up_to = 5;
    }
    
    if (n >= cache_size) {
        idx_t new_size = n + 100;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        cache[i] = 9*cache[i-1] - 28*cache[i-2] + 35*cache[i-3] - 15*cache[i-4] + cache[i-5];
    }
    if (n > computed_up_to) {
        computed_up_to = n;
    }
    
    return cache[n];
}
