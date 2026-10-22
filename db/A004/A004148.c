/*
 * A004148  Generalized Catalan numbers: a(n+1) = a(n) + Sum_{k=1..n-1}
 *          a(k)*a(n-1-k).
 * https://oeis.org/A004148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n+1) = a(n) + Sum_{k=1..n-1} a(k)*a(n-1-k), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 28, u64 n <= 52, u128 n <= 99,
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
    
    if (n >= memo_len) {
        idx_t new_len = n + 1;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        
        if (memo) {
            memcpy(new_memo, memo, memo_len * sizeof(seq_t));
            free(memo);
        }
        
        if (new_len > 0) new_memo[0] = 1;
        if (new_len > 1) new_memo[1] = 1;
        
        idx_t start = memo_len > 2 ? memo_len : 2;
        for (idx_t i = start; i < new_len; i++) {
            new_memo[i] = new_memo[i-1];
            for (idx_t k = 1; k < i - 1; k++) {
                new_memo[i] += new_memo[k] * new_memo[i-2-k];
            }
        }
        
        memo = new_memo;
        memo_len = new_len;
    }
    
    return memo[n];
}
