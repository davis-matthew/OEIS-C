/*
 * A003994  Sequence b_4 (n) arising from homology of partitions with even
 *          number of blocks.
 * https://oeis.org/A003994
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - a(n-2) - 8*n^2 + 24*n - 10 +
 *            3^(n-3)*(32*n-80), with a(1)=a(2)=0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;
    static idx_t computed_up_to = 2;
    
    if (n <= 2) return 0;
    
    if (memo == NULL || memo_len <= n) {
        idx_t new_len = (n < 100) ? 100 : (n + 100);
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        
        if (memo != NULL) {
            memcpy(new_memo, memo, memo_len * sizeof(seq_t));
            free(memo);
        }
        
        for (idx_t i = (memo == NULL ? 0 : memo_len); i < new_len; i++) {
            new_memo[i] = 0;
        }
        
        memo = new_memo;
        memo_len = new_len;
    }
    
    memo[0] = 0;
    memo[1] = 0;
    memo[2] = 0;
    
    seq_t pow3 = 1;
    if (computed_up_to >= 2) {
        for (idx_t j = 0; j < computed_up_to - 2; j++) {
            pow3 = pow3 * 3;
        }
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t ni = (seq_t)i;
        seq_t poly_neg = 8*ni*ni - 24*ni + 10;
        seq_t exp = pow3 * (32*ni - 80);
        
        memo[i] = 6*memo[i-1] - memo[i-2] + exp - poly_neg;
        
        pow3 = pow3 * 3;
    }
    
    if (n > computed_up_to) {
        computed_up_to = n;
    }
    
    return memo[n];
}
