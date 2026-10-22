/*
 * A001719  Generalized Stirling numbers, [n+8,8]_4.
 * https://oeis.org/A001719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*(n+5)*a(n-1) - 5*(2*n^2+18*n+41)*a(n-2) + 5*(n+4)*(2*n^2+16*n+33)*a(n-3) - (5*n^4+70*n^3+370*n^2+875*n+781)*a(n-4) + (n+3)^5*a(n-5), with a(0)=1, a(1)=30, a(2)=625, a(3)=11515, a(4)=203889 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_computed = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_len = (n >= 32) ? (n + 1) : 32;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        
        cache[0] = 1;
        cache[1] = 30;
        cache[2] = 625;
        cache[3] = 11515;
        cache[4] = 203889;
        cache_computed = 4;
    }
    
    if (n >= cache_len) {
        idx_t new_len = cache_len * 2;
        while (new_len <= n) new_len *= 2;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }
    
    for (idx_t i = cache_computed + 1; i <= n; i++) {
        seq_t is = (seq_t)i;
        seq_t ip3 = is + 3;
        seq_t ip4 = is + 4;
        seq_t ip5 = is + 5;
        
        seq_t ip3_5 = ip3 * ip3 * ip3 * ip3 * ip3;
        
        seq_t c1 = 5 * ip5;
        seq_t c2 = 5 * (2*is*is + 18*is + 41);
        seq_t c3 = 5 * ip4 * (2*is*is + 16*is + 33);
        seq_t c4 = 5*is*is*is*is + 70*is*is*is + 370*is*is + 875*is + 781;
        
        cache[i] = c1*cache[i-1] - c2*cache[i-2] + c3*cache[i-3] - c4*cache[i-4] + ip3_5*cache[i-5];
    }
    
    if (n > cache_computed) cache_computed = n;
    
    return cache[n];
}
