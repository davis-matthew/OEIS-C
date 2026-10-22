/*
 * A004306  Rook polynomials; fourth column of A008305.
 * https://oeis.org/A004306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*(t(n+1) + 2*t(n) + 3*t(n-1) + 1) with t(n) = A000073(n)
 *            tribonacci, a(0)=1, a(1)=1, a(2)=2, a(3)=6
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static seq_t *tribonacci_cache = NULL;
static idx_t tribonacci_len = 0;

static seq_t tribonacci(idx_t n) {
    if (n < tribonacci_len) {
        return tribonacci_cache[n];
    }
    
    if (tribonacci_cache == NULL) {
        tribonacci_len = 10;
        tribonacci_cache = (seq_t *)malloc(tribonacci_len * sizeof(seq_t));
        tribonacci_cache[0] = 0;
        tribonacci_cache[1] = 0;
        tribonacci_cache[2] = 1;
        for (idx_t i = 3; i < tribonacci_len; i++) {
            tribonacci_cache[i] = tribonacci_cache[i-1] + tribonacci_cache[i-2] + tribonacci_cache[i-3];
        }
    }
    
    if (n >= tribonacci_len) {
        idx_t new_len = tribonacci_len * 2;
        while (new_len <= n) {
            new_len *= 2;
        }
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        memcpy(new_cache, tribonacci_cache, tribonacci_len * sizeof(seq_t));
        
        for (idx_t i = tribonacci_len; i < new_len; i++) {
            new_cache[i] = new_cache[i-1] + new_cache[i-2] + new_cache[i-3];
        }
        
        free(tribonacci_cache);
        tribonacci_cache = new_cache;
        tribonacci_len = new_len;
    }
    
    return tribonacci_cache[n];
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 6;
    
    seq_t t_n_plus_1 = tribonacci(n + 1);
    seq_t t_n = tribonacci(n);
    seq_t t_n_minus_1 = tribonacci(n - 1);
    
    return 2 * (t_n_plus_1 + 2 * t_n + 3 * t_n_minus_1 + 1);
}
