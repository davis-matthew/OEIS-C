/*
 * A005185  Hofstadter Q-sequence: a(1) = a(2) = 1; a(n) = a(n-a(n-1)) +
 *          a(n-a(n-2)) for n > 2.
 * https://oeis.org/A005185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, look
 * Formula:   a(n) = a(n - a(n-1)) + a(n - a(n-2)) for n > 2, with a(1) = a(2)
 *            = 1
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_zero.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n <= 0) return 0;
    if (n <= 2) return 1;
    
    if (n >= memo_len) {
        idx_t old_len = memo_len;
        idx_t new_len = n + 1;
        memo = grow_zero(memo, old_len, new_len);
        
        if (old_len == 0) {
            memo[1] = 1;
            memo[2] = 1;
            old_len = 3;
        }
        
        for (idx_t i = old_len; i <= n; i++) {
            idx_t idx1 = i - (idx_t)memo[i-1];
            idx_t idx2 = i - (idx_t)memo[i-2];
            memo[i] = memo[idx1] + memo[idx2];
        }
        
        memo_len = new_len;
    }
    
    return memo[n];
}
