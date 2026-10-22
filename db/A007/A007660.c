/*
 * A007660  a(n) = a(n-1)*a(n-2) + 1 with a(0) = a(1) = 0.
 * https://oeis.org/A007660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*a(n-2) + 1, a(0) = 0, a(1) = 0
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 11, u64 n <= 12, u128 n <= 13,
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
        // First call: initialize and compute up to n
        memo_len = (n >= 2) ? n + 1 : 2;
        memo = (seq_t *)malloc(sizeof(seq_t) * memo_len);
        memo[0] = 0;
        memo[1] = 0;
        for (idx_t i = 2; i < memo_len; i++) {
            memo[i] = memo[i-1] * memo[i-2] + 1;
        }
        return memo[n];
    }
    
    if (n < memo_len) {
        return memo[n];
    }
    
    // Grow array if needed
    idx_t old_len = memo_len;
    memo_len = n + 1;
    memo = (seq_t *)realloc(memo, sizeof(seq_t) * memo_len);
    
    // Compute new values from old_len to n
    for (idx_t i = old_len; i < memo_len; i++) {
        memo[i] = memo[i-1] * memo[i-2] + 1;
    }
    
    return memo[n];
}
