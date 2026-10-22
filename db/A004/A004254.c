/*
 * A004254  a(n) = 5*a(n-1) - a(n-2) for n > 1, a(0) = 0, a(1) = 1.
 * https://oeis.org/A004254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t highest_computed = 0;

seq_t a(idx_t n) {
    if (memo == NULL) {
        memo_len = 128;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[0] = 0;
        memo[1] = 1;
        highest_computed = 1;
    }
    
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    if (n <= highest_computed) {
        return memo[n];
    }
    
    while (n >= memo_len) {
        memo_len *= 2;
        memo = (seq_t *)realloc(memo, memo_len * sizeof(seq_t));
    }
    
    for (idx_t i = highest_computed + 1; i <= n; i++) {
        memo[i] = 5 * memo[i - 1] - memo[i - 2];
    }
    
    highest_computed = n;
    return memo[n];
}
