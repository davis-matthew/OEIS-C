/*
 * A007482  a(n) is the number of subsequences of [ 1, ..., 2n ] in which each
 *          odd number has an even neighbor.
 * https://oeis.org/A007482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2), a(0) = 1, a(1) = 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t memo_computed = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 3;
    
    if (n >= memo_len) {
        idx_t new_len = (n < 10) ? 10 : (n + 1);
        memo = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        memo_len = new_len;
    }
    
    if (memo_computed == 0) {
        memo[0] = 1;
        memo[1] = 3;
        memo_computed = 2;
    }
    
    for (idx_t i = memo_computed; i <= n; i++) {
        memo[i] = (seq_t)3 * memo[i-1] + (seq_t)2 * memo[i-2];
    }
    
    if (memo_computed <= n) {
        memo_computed = n + 1;
    }
    
    return memo[n];
}
