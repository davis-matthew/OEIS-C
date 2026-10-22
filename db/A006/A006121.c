/*
 * A006121  Sum of Gaussian binomial coefficients [ n,k ] for q=7.
 * https://oeis.org/A006121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + (7^(n-1) - 1)*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t memo_computed = 0;

static seq_t pow7(idx_t k) {
    seq_t result = 1;
    seq_t base = 7;
    while (k > 0) {
        if (k & 1) {
            result = result * base;
        }
        base = base * base;
        k >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    
    if (memo == NULL) {
        memo_len = n + 10;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[0] = 1;
        memo[1] = 2;
        memo_computed = 2;
    }
    
    if (n >= memo_len) {
        idx_t new_len = n + 10;
        seq_t *new_memo = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        memo = new_memo;
        memo_len = new_len;
    }
    
    if (n >= memo_computed) {
        for (idx_t i = memo_computed; i <= n; i++) {
            seq_t term1 = 2 * memo[i - 1];
            seq_t pow7_val = pow7(i - 1);
            seq_t term2 = (pow7_val - 1) * memo[i - 2];
            memo[i] = term1 + term2;
        }
        memo_computed = n + 1;
    }
    
    return memo[n];
}
