/*
 * A005210  a(n) = |a(n-1) + 2a(n-2) - n|.
 * https://oeis.org/A005210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = |a(n-1) + 2*a(n-2) - n|, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 342, u16 n <= 9926, u32 n <= 9926, u64 n <= 9926,
 *            u128 n <= 9926, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (cache == NULL) {
        cache_len = (n >= 10) ? n : 10;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        
        cache[0] = 1;  // a(1)
        cache[1] = 1;  // a(2)
        
        for (idx_t i = 3; i <= cache_len; i++) {
            seq_t prev1 = cache[i - 2];
            seq_t prev2 = cache[i - 3];
            seq_t n_seq = (seq_t)i;
            seq_t sum = prev1 + 2 * prev2;
            
            if (sum >= n_seq) {
                cache[i - 1] = sum - n_seq;
            } else {
                cache[i - 1] = n_seq - sum;
            }
        }
    } else if (n > cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 100;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        
        for (idx_t i = old_len + 1; i <= cache_len; i++) {
            seq_t prev1 = cache[i - 2];
            seq_t prev2 = cache[i - 3];
            seq_t n_seq = (seq_t)i;
            seq_t sum = prev1 + 2 * prev2;
            
            if (sum >= n_seq) {
                cache[i - 1] = sum - n_seq;
            } else {
                cache[i - 1] = n_seq - sum;
            }
        }
    }
    
    return cache[n - 1];
}
