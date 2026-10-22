/*
 * A002137  Number of n X n symmetric matrices with nonnegative integer
 *          entries, trace 0 and all row sums 2.
 * https://oeis.org/A002137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (n-1)*(a(n-1)+a(n-2)) - (n-1)*(n-2)*a(n-3)/2, a(0) = 1,
 *            a(1) = 0, a(2) = 1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_up_to = 2;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n == 2) return 1;
    
    if (cache == NULL) {
        cache_len = (n < 100) ? 100 : n + 10;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 1;
    } else if (cache_len <= n) {
        idx_t new_len = n + 10;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t i_minus_1 = (seq_t)(i - 1);
        seq_t i_minus_2 = (seq_t)(i - 2);
        seq_t term1 = i_minus_1 * (cache[i-1] + cache[i-2]);
        seq_t term2 = i_minus_1 * i_minus_2 * cache[i-3] / 2;
        cache[i] = term1 - term2;
    }
    
    if (n > computed_up_to) {
        computed_up_to = n;
    }
    
    return cache[n];
}
