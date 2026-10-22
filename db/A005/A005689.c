/*
 * A005689  Number of Twopins positions.
 * https://oeis.org/A005689
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-6) for n >= 12
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_up_to = 5;

seq_t a(idx_t n) {
    if (n < 6) return 0;
    
    static const seq_t init[6] = {1, 2, 4, 7, 11, 16};
    
    if (n <= 11) {
        return init[n - 6];
    }
    
    if (cache == NULL) {
        cache_len = 8192;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 6; i <= 11; i++) {
            cache[i] = init[i - 6];
        }
        computed_up_to = 11;
    }
    
    if (n >= cache_len) {
        idx_t new_len = cache_len;
        while (new_len <= n) {
            new_len *= 2;
        }
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        cache[i] = 2 * cache[i-1] - cache[i-2] + cache[i-6];
    }
    computed_up_to = n;
    
    return cache[n];
}
