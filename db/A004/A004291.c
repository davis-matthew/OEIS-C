/*
 * A004291  Expansion of (1 + 2*x + x^2)/(1 - 10*x + x^2).
 * https://oeis.org/A004291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - a(n-2) for n >= 3, with a(0) = 1, a(1) = 12,
 *            a(2) = 120
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (memo != NULL && n < memo_len) {
        return memo[n];
    }
    
    if (memo == NULL) {
        // Initial allocation
        memo_len = (n < 3) ? 3 : (n + 1);
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        if (memo == NULL) return (seq_t)0;
        
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)12;
        memo[2] = (seq_t)120;
        
        for (idx_t i = 3; i <= n; i++) {
            memo[i] = (seq_t)10 * memo[i-1] - memo[i-2];
        }
    } else {
        // Reallocate to accommodate larger n
        idx_t old_len = memo_len;
        memo_len = n + 1;
        seq_t *new_memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        if (new_memo == NULL) return (seq_t)0;
        
        for (idx_t i = 0; i < old_len; i++) {
            new_memo[i] = memo[i];
        }
        free(memo);
        memo = new_memo;
        
        for (idx_t i = old_len; i <= n; i++) {
            memo[i] = (seq_t)10 * memo[i-1] - memo[i-2];
        }
    }
    
    return memo[n];
}
