/*
 * A000902  Expansion of e.g.f. (1/2)*(exp(2*x + x^2) + 1).
 * https://oeis.org/A000902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*a(n-1) + (2n-2)*a(n-2) for n >= 3, a(0) = 1, a(1) = 1,
 *            a(2) = 3
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    if (n <= 2) {
        if (n == 0 || n == 1) return 1;
        return 3;
    }
    
    static seq_t *memo = NULL;
    static idx_t memo_cap = 0;
    static idx_t memo_len = 0;
    
    if (memo == NULL) {
        memo_cap = 16;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        memo[0] = 1;
        memo[1] = 1;
        memo[2] = 3;
        memo_len = 3;
    }
    
    if (n < memo_len) {
        return memo[n];
    }
    
    // Expand if necessary
    while (n >= memo_cap) {
        memo_cap *= 2;
        memo = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
    }
    
    // Compute missing values
    for (idx_t i = memo_len; i <= n; i++) {
        memo[i] = 2 * memo[i-1] + (2*i - 2) * memo[i-2];
    }
    memo_len = n + 1;
    
    return memo[n];
}
