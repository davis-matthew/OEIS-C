/*
 * A003770  Number of 2-factors in K_4 X P_n.
 * https://oeis.org/A003770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) + 8*a(n-2) - 12*a(n-3), n>3; a(1)=3, a(2)=42,
 *            a(3)=474
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)42;
    if (n == 3) return (seq_t)474;
    
    if (cache == NULL) {
        cache_len = (n < 100) ? 100 : n + 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)42;
        cache[3] = (seq_t)474;
        for (idx_t i = 4; i < cache_len; i++) {
            cache[i] = 0;
        }
    } else if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        for (idx_t i = old_len; i < cache_len; i++) {
            cache[i] = 0;
        }
    }
    
    for (idx_t i = 4; i <= n; i++) {
        if (cache[i] == 0) {
            cache[i] = 11 * cache[i-1] + 8 * cache[i-2] - 12 * cache[i-3];
        }
    }
    
    return cache[n];
}
