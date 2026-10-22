/*
 * A005828  a(n) = 2*a(n-1)^2 - 1, a(0) = 4, a(1) = 31.
 * https://oeis.org/A005828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)^2 - 1, a(0) = 4, a(1) = 31
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A_grow.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (memo == NULL) {
        memo = A_grow(NULL, 0, 2);
        memo[0] = 4;
        memo[1] = 31;
        memo_len = 2;
    }
    
    if (n >= memo_len) {
        memo = A_grow(memo, memo_len, n + 1);
        for (idx_t i = memo_len; i <= n; i++) {
            seq_t prev = memo[i - 1];
            memo[i] = 2 * prev * prev - 1;
        }
        memo_len = n + 1;
    }
    
    return memo[n];
}
