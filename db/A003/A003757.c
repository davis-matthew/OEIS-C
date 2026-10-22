/*
 * A003757  Number of perfect matchings (or domino tilings) in D_4 X P_(n-1).
 * https://oeis.org/A003757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 6*a(n-2) + a(n-3) - a(n-4), a(0)=0, a(1)=1,
 *            a(2)=1, a(3)=6, a(4)=13
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static void ensure_computed(idx_t n) {
    if (memo == NULL) {
        memo = (seq_t *)malloc(5 * sizeof(seq_t));
        memo[0] = 0;
        memo[1] = 1;
        memo[2] = 1;
        memo[3] = 6;
        memo[4] = 13;
        memo_len = 5;
    }
    
    if (n < memo_len) return;
    
    idx_t new_len = memo_len;
    while (new_len <= n) new_len <<= 1;
    
    memo = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
    
    for (idx_t i = memo_len; i <= n; i++) {
        memo[i] = memo[i-1] + 6*memo[i-2] + memo[i-3] - memo[i-4];
    }
    memo_len = n + 1;
}

seq_t a(idx_t n) {
    ensure_computed(n);
    return memo[n];
}
