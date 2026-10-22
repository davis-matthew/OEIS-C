/*
 * A001054  a(n) = a(n-1)*a(n-2) - 1.
 * https://oeis.org/A001054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1)*a(n-2) - 1, a(0) = 0, a(1) = 1
 * Range:     i8 n <= 8, i16 n <= 10, i32 n <= 11, i64 n <= 13, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t computed_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    
    // Initialize or resize memo
    if (memo == NULL) {
        memo_len = (n < 100) ? 100 : (n + 10);
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[0] = (seq_t)0;
        memo[1] = (seq_t)1;
        computed_len = 2;
    } else if (n >= memo_len) {
        memo_len = n + 10;
        memo = (seq_t *)realloc(memo, memo_len * sizeof(seq_t));
    }
    
    // Compute values up to n
    for (idx_t i = computed_len; i <= n; i++) {
        memo[i] = memo[i-1] * memo[i-2] - (seq_t)1;
    }
    if (n >= computed_len) {
        computed_len = n + 1;
    }
    
    return memo[n];
}
