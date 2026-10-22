/*
 * A003819  a(1)=a(2)=1, a(n+1) = (a(n)^4 +1)/a(n-1).
 * https://oeis.org/A003819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = (a(n)^4 + 1) / a(n-1), a(1) = 1, a(2) = 1
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 6,
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
    if (cache == NULL) {
        cache_len = (n < 1000) ? 1000 : n * 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        computed_up_to = 2;
    }
    
    if (n > cache_len) {
        cache_len = n * 2;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t a_i_minus_2 = cache[i - 3];
        seq_t a_i_minus_1 = cache[i - 2];
        seq_t pow4 = a_i_minus_1 * a_i_minus_1;
        pow4 = pow4 * pow4;
        cache[i - 1] = (pow4 + 1) / a_i_minus_2;
    }
    
    if (n > computed_up_to) {
        computed_up_to = n;
    }
    
    return cache[n - 1];
}
