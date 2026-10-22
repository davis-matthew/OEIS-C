/*
 * A005826  Worst case of a Jacobi symbol algorithm.
 * https://oeis.org/A005826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-3) + 4*a(n-4), a(0) = 1, a(1) = 5, a(2)
 *            = 31, a(3) = 141
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (n <= 3) {
        if (n == 0) return (seq_t)1;
        if (n == 1) return (seq_t)5;
        if (n == 2) return (seq_t)31;
        return (seq_t)141;
    }
    
    if (cache == NULL) {
        cache_len = 1024;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)31;
        cache[3] = (seq_t)141;
        computed_up_to = 3;
    }
    
    while (n >= cache_len) {
        cache_len *= 2;
        seq_t *new_cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        memcpy(new_cache, cache, (computed_up_to + 1) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
    }
    
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; i++) {
            cache[i] = (seq_t)5 * cache[i - 1] - (seq_t)10 * cache[i - 3] + (seq_t)4 * cache[i - 4];
        }
        computed_up_to = n;
    }
    
    return cache[n];
}
