/*
 * A006162  a(n)=a(a(n-1)-2)+a(n+2-a(n-1)).
 * https://oeis.org/A006162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(n-1)-2) + a(n+2-a(n-1)), a(1) = 1, a(2) = 1, a(3) =
 *            2, a(4) = 3
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n <= 0) return 0;
    
    if (cache == NULL) {
        cache_len = (n < 256) ? 256 : (n + 1) * 2;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 2;
        cache[4] = 3;
    }
    
    if (n >= cache_len) {
        idx_t new_len = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)calloc(new_len, sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    for (idx_t i = 5; i <= n; i++) {
        if (cache[i] == 0) {
            seq_t a_i_minus_1 = cache[i - 1];
            idx_t idx1 = (idx_t)a_i_minus_1 - 2;
            idx_t idx2 = (idx_t)i + 2 - (idx_t)a_i_minus_1;
            cache[i] = cache[idx1] + cache[idx2];
        }
    }
    
    return cache[n];
}
