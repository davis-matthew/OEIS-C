/*
 * A001042  a(n) = a(n-1)^2 - a(n-2)^2.
 * https://oeis.org/A001042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-1)^2 - a(n-2)^2, a(0) = 1, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_up_to = 1;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    
    if (cache == NULL) {
        cache_len = (n < 100) ? 100 : (n + 1) * 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 2;
    }
    
    if (n >= cache_len) {
        idx_t new_len = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        cache[i] = cache[i-1] * cache[i-1] - cache[i-2] * cache[i-2];
    }
    if (n > computed_up_to) {
        computed_up_to = n;
    }
    
    return cache[n];
}
