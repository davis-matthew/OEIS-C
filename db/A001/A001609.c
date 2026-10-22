/*
 * A001609  a(1) = a(2) = 1, a(3) = 4; thereafter a(n) = a(n-1) + a(n-3).
 * https://oeis.org/A001609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3), a(1) = 1, a(2) = 1, a(3) = 4
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 58, u64 n <= 116,
 *            u128 n <= 232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static void ensure_computed(idx_t n) {
    if (n >= memo_len) {
        idx_t new_len = (n < 100) ? 100 : n + 10;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        
        for (idx_t i = 0; i < new_len; i++) {
            if (i < memo_len) {
                new_memo[i] = memo[i];
            } else {
                new_memo[i] = 0;
            }
        }
        
        if (memo) free(memo);
        memo = new_memo;
        memo_len = new_len;
    }
    
    if (memo[1] == 0) {
        memo[1] = 1;
        memo[2] = 1;
        memo[3] = 4;
    }
    
    for (idx_t i = 4; i <= n; i++) {
        if (memo[i] == 0) {
            memo[i] = memo[i-1] + memo[i-3];
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    ensure_computed(n);
    return memo[n];
}
