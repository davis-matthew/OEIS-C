/*
 * A005232  Expansion of (1-x+x^2)/((1-x)^2*(1-x^2)*(1-x^4)).
 * https://oeis.org/A005232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   (1+x^3)/((1-x)*(1-x^2)^2*(1-x^4))
 * Range:     u8 n <= 19, u16 n <= 143, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (memo_len <= n) {
        idx_t new_len = (n < 1024) ? 1024 : (n + 1);
        
        if (!memo) {
            memo = (seq_t *)malloc(new_len * sizeof(seq_t));
            memo[0] = 1;
            memo[1] = 1;
            memo[2] = 3;
            memo[3] = 4;
            memo[4] = 8;
            memo[5] = 10;
            memo[6] = 16;
            memo[7] = 20;
            memo[8] = 29;
            
            for (idx_t i = 9; i < new_len; i++) {
                memo[i] = memo[i-1] + 2*memo[i-2] - 2*memo[i-3] - 2*memo[i-6] + 2*memo[i-7] + memo[i-8] - memo[i-9];
            }
            memo_len = new_len;
        } else {
            while (new_len <= n) new_len *= 2;
            memo = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
            
            for (idx_t i = memo_len; i < new_len; i++) {
                memo[i] = memo[i-1] + 2*memo[i-2] - 2*memo[i-3] - 2*memo[i-6] + 2*memo[i-7] + memo[i-8] - memo[i-9];
            }
            memo_len = new_len;
        }
    }
    
    return memo[n];
}
