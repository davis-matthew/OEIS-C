/*
 * A000806  Bessel polynomial y_n(-1).
 * https://oeis.org/A000806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = (-2*n+1)*a(n-1) + a(n-2), a(0) = 1, a(1) = 0
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 28,
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
    if (n == 0) return 1;
    if (n == 1) return 0;
    
    if (cache == NULL) {
        cache_size = 1000;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 0;
        computed_up_to = 1;
    }
    
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n) new_size *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = new_cache;
        cache_size = new_size;
    }
    
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; i++) {
            seq_t coeff = -2 * (seq_t)i + 1;
            cache[i] = coeff * cache[i-1] + cache[i-2];
        }
        computed_up_to = n;
    }
    
    return cache[n];
}
