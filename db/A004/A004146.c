/*
 * A004146  Alternate Lucas numbers - 2.
 * https://oeis.org/A004146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3), n>=3, a(0)=0, a(1)=1,
 *            a(2)=5
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;
    
    if (n >= memo_len) {
        idx_t new_len = n + 1;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        
        for (idx_t i = 0; i < memo_len; i++) {
            new_memo[i] = memo[i];
        }
        if (memo != NULL) {
            free(memo);
        }
        
        for (idx_t i = memo_len; i < new_len; i++) {
            if (i == 0) {
                new_memo[i] = (seq_t)0;
            } else if (i == 1) {
                new_memo[i] = (seq_t)1;
            } else if (i == 2) {
                new_memo[i] = (seq_t)5;
            } else {
                new_memo[i] = (seq_t)4 * new_memo[i-1] - (seq_t)4 * new_memo[i-2] + new_memo[i-3];
            }
        }
        
        memo = new_memo;
        memo_len = new_len;
    }
    
    return memo[n];
}
