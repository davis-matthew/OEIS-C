/*
 * A003683  a(n) = 2^(n-1)*(2^n - (-1)^n)/3.
 * https://oeis.org/A003683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;
    
    if (memo == NULL) {
        memo_len = 2;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[0] = (seq_t)0;
        memo[1] = (seq_t)1;
    }
    
    if (n >= memo_len) {
        idx_t new_len = (n + 1) * 2;
        seq_t *new_memo = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        memo = new_memo;
        
        for (idx_t i = memo_len; i < new_len; i++) {
            memo[i] = 2 * memo[i-1] + 8 * memo[i-2];
        }
        
        memo_len = new_len;
    }
    
    return memo[n];
}
