/*
 * A001640  A Fielder sequence.
 * https://oeis.org/A001640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) + a(n-5) + a(n-6) for n >= 7,
 *            with a(1)=1, a(2)=1, a(3)=4, a(4)=9, a(5)=16, a(6)=28
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 81, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t last_computed = 0;

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 4;
    if (n == 4) return 9;
    if (n == 5) return 16;
    if (n == 6) return 28;
    
    if (memo == NULL) {
        memo_len = 128;
        memo = (seq_t *)malloc((memo_len + 1) * sizeof(seq_t));
        memo[1] = 1;
        memo[2] = 1;
        memo[3] = 4;
        memo[4] = 9;
        memo[5] = 16;
        memo[6] = 28;
        last_computed = 6;
    }
    
    if (n >= memo_len) {
        idx_t newlen = n + 128;
        memo = (seq_t *)realloc(memo, (newlen + 1) * sizeof(seq_t));
        memo_len = newlen;
    }
    
    for (idx_t i = last_computed + 1; i <= n; i++) {
        memo[i] = memo[i-1] + memo[i-3] + memo[i-4] + memo[i-5] + memo[i-6];
    }
    if (n > last_computed) last_computed = n;
    
    return memo[n];
}
