/*
 * A006570  From trees with valency <= 3.
 * https://oeis.org/A006570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   0 = (1 + x) * A(x)^2 + (2*x - 2) * A(x) + (1 + x) * A(x^2) + 2*x
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;
    
    if (memo == NULL) {
        memo_len = (n < 1000) ? 1000 : n;
        memo = (seq_t *)malloc(sizeof(seq_t) * (memo_len + 1));
        memo[0] = 0;
        memo[1] = 1;
        
        for (idx_t i = 2; i <= memo_len; i++) {
            seq_t S1 = 0, S2 = 0, S3 = 0, S4 = 0;
            
            for (idx_t j = 1; j < i; j++) {
                S1 += memo[j] * memo[i - j];
            }
            
            for (idx_t j = 1; j < i - 1; j++) {
                S2 += memo[j] * memo[i - 1 - j];
            }
            
            if (i % 2 == 0) {
                S3 = memo[i / 2];
            }
            
            if (i % 2 == 1) {
                S4 = memo[(i - 1) / 2];
            }
            
            memo[i] = (S1 + S2 + 2 * memo[i - 1] + S3 + S4) / 2;
        }
    } else if (n > memo_len) {
        idx_t old_len = memo_len;
        memo_len = (n > memo_len * 2) ? n : (memo_len * 2);
        memo = (seq_t *)realloc(memo, sizeof(seq_t) * (memo_len + 1));
        
        for (idx_t i = old_len + 1; i <= memo_len; i++) {
            seq_t S1 = 0, S2 = 0, S3 = 0, S4 = 0;
            
            for (idx_t j = 1; j < i; j++) {
                S1 += memo[j] * memo[i - j];
            }
            
            for (idx_t j = 1; j < i - 1; j++) {
                S2 += memo[j] * memo[i - 1 - j];
            }
            
            if (i % 2 == 0) {
                S3 = memo[i / 2];
            }
            
            if (i % 2 == 1) {
                S4 = memo[(i - 1) / 2];
            }
            
            memo[i] = (S1 + S2 + 2 * memo[i - 1] + S3 + S4) / 2;
        }
    }
    
    return memo[n];
}
