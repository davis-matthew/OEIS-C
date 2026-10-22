/*
 * A003269  a(n) = a(n-1) + a(n-4) with a(0) = 0, a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A003269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4), a(0) = 0, a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 20, u16 n <= 37, u32 n <= 71, u64 n <= 140,
 *            u128 n <= 278, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;
    static idx_t computed = 3;
    
    if (n <= 3) {
        return (n == 0) ? 0 : 1;
    }
    
    if (memo == NULL) {
        memo_len = (n < 100) ? 100 : n + 1;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[0] = 0;
        memo[1] = 1;
        memo[2] = 1;
        memo[3] = 1;
    }
    
    while (n >= memo_len) {
        memo_len = memo_len * 2;
        memo = (seq_t *)realloc(memo, memo_len * sizeof(seq_t));
    }
    
    for (idx_t i = computed + 1; i <= n; i++) {
        memo[i] = memo[i-1] + memo[i-4];
    }
    
    if (n > computed) {
        computed = n;
    }
    
    return memo[n];
}
