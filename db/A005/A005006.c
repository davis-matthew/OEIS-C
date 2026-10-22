/*
 * A005006  Davenport-Schinzel numbers of degree n on 5 symbols.
 * https://oeis.org/A005006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + f(n), where f has period 8 based on n
 *            mod 8
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (memo == NULL) {
        memo_len = n + 128;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[1] = 1;
        memo[2] = 5;
    }
    
    if (n >= memo_len) {
        idx_t old_len = memo_len;
        memo_len = n + 128;
        seq_t *new_memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        for (idx_t i = 0; i < old_len; i++) {
            new_memo[i] = memo[i];
        }
        free(memo);
        memo = new_memo;
    }
    
    static const seq_t f_table[] = {5, -4, 4, 0, 4, -3, 7, -5};
    
    for (idx_t i = 3; i <= n; i++) {
        if (i == 1) continue;
        seq_t f_val = f_table[i % 8];
        memo[i] = 2 * memo[i-1] - memo[i-2] + f_val;
    }
    
    return memo[n];
}
