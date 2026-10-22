/*
 * A002370  a(n) = (2*n-1)^2 * a(n-1) - 3*C(2*n-1,3) * a(n-2) for n>1; a(0) =
 *          a(1) = 1.
 * https://oeis.org/A002370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)^2 * a(n-1) - 3*C(2*n-1,3) * a(n-2) for n>1; a(0)
 *            = a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
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
        memo_len = 2;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        if (!memo) return (seq_t)0;
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)1;
    }
    
    if (n < memo_len)
        return memo[n];
    
    // Extend memo to have capacity for index n
    idx_t new_len = memo_len;
    while (new_len <= n) new_len *= 2;
    
    seq_t *new_memo = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
    if (!new_memo) return (seq_t)0;
    memo = new_memo;
    
    // Compute values from memo_len to n using the recurrence
    for (idx_t i = memo_len; i <= n; i++) {
        seq_t two_i_minus_1 = (seq_t)(2 * i - 1);
        seq_t coeff1 = two_i_minus_1 * two_i_minus_1;
        
        // C(2*i-1, 3) = (2*i-1)*(2*i-2)*(2*i-3)/6
        seq_t c = ((seq_t)(2 * i - 1) * (seq_t)(2 * i - 2) * (seq_t)(2 * i - 3)) / (seq_t)6;
        
        // a(i) = (2*i-1)^2 * a(i-1) - 3*C(2*i-1,3) * a(i-2)
        memo[i] = coeff1 * memo[i - 1] - (seq_t)3 * c * memo[i - 2];
    }
    
    memo_len = n + 1;
    return memo[n];
}
