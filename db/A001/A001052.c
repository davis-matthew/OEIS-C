/*
 * A001052  a(n) = n*(n-1)*a(n-1)/2 + a(n-2), a(0) = 1, a(1) = 2.
 * https://oeis.org/A001052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*a(n-1)/2 + a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t computed = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    
    if (memo_len <= n) {
        idx_t new_len = (n + 1) * 2;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (memo) {
            for (idx_t i = 0; i < memo_len; i++) {
                new_memo[i] = memo[i];
            }
            free(memo);
        }
        memo = new_memo;
        memo_len = new_len;
        
        if (computed == 0) {
            memo[0] = 1;
            memo[1] = 2;
            computed = 1;
        }
    }
    
    for (idx_t i = computed + 1; i <= n; i++) {
        seq_t i_seq = (seq_t)i;
        seq_t i_minus_1_seq = (seq_t)(i - 1);
        
        if (i_seq % 2 == 0) {
            memo[i] = (i_seq / 2) * i_minus_1_seq * memo[i-1] + memo[i-2];
        } else {
            memo[i] = i_seq * (i_minus_1_seq / 2) * memo[i-1] + memo[i-2];
        }
    }
    
    if (computed < n) computed = n;
    
    return memo[n];
}
