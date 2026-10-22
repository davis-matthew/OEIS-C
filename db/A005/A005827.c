/*
 * A005827  Worst case of a Jacobi symbol algorithm.
 * https://oeis.org/A005827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-3) + 4*a(n-4), a(0)=1, a(1)=3, a(2)=13,
 *            a(3)=57
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)57;
    
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)57;
    }
    
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        
        for (idx_t i = cache_len; i <= n; i++) {
            cache[i] = 5 * cache[i-1] - 10 * cache[i-3] + 4 * cache[i-4];
        }
        cache_len = new_len;
    }
    
    return cache[n];
}
