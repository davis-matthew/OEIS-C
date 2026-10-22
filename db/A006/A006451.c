/*
 * A006451  Numbers k such that k*(k+1)/2 + 1 is a square.
 * https://oeis.org/A006451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) - a(n-4) + 2, a(0)=0, a(1)=2, a(2)=5, a(3)=15
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;
    
    if (memo == NULL) {
        memo_size = 4;
        memo = (seq_t *)malloc(memo_size * sizeof(seq_t));
        memo[0] = 0;
        memo[1] = 2;
        memo[2] = 5;
        memo[3] = 15;
    }
    
    while (n >= memo_size) {
        idx_t old_size = memo_size;
        memo_size *= 2;
        seq_t *new_memo = (seq_t *)malloc(memo_size * sizeof(seq_t));
        for (idx_t i = 0; i < old_size; i++) {
            new_memo[i] = memo[i];
        }
        free(memo);
        memo = new_memo;
        
        for (idx_t i = old_size; i < memo_size; i++) {
            memo[i] = 6 * memo[i-2] - memo[i-4] + 2;
        }
    }
    
    return memo[n];
}
