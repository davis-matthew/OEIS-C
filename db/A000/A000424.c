/*
 * A000424  Differences of reciprocals of unity.
 * https://oeis.org/A000424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n^2 + 3*n + 1)*a(n-1) - 3*n^4*a(n-2) +
 *            (n-1)^3*n^3*a(n-3), with a(1)=7, a(2)=85, a(3)=1660
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 1) return 7;
    if (n == 2) return 85;
    if (n == 3) return 1660;
    if (n == 0) return 0;
    
    if (!cache || cache_len <= n) {
        idx_t new_len = (n > 1000) ? n + 1000 : n * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0;
        
        memset(new_cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        
        cache = new_cache;
        cache_len = new_len;
    }
    
    cache[1] = 7;
    cache[2] = 85;
    cache[3] = 1660;
    
    for (idx_t i = 4; i <= n; i++) {
        if (cache[i] != 0) continue;
        
        seq_t n_seq = (seq_t)i;
        seq_t n2 = n_seq * n_seq;
        seq_t n3 = n2 * n_seq;
        seq_t n4 = n2 * n2;
        seq_t n_minus_1 = n_seq - 1;
        
        seq_t coeff1 = 3*n2 + 3*n_seq + 1;
        seq_t coeff2 = 3*n4;
        seq_t coeff3 = n_minus_1*n_minus_1*n_minus_1*n3;
        
        cache[i] = coeff1*cache[i-1] - coeff2*cache[i-2] + coeff3*cache[i-3];
    }
    
    return cache[n];
}
