/*
 * A003318  a(n+1) = 1 + a( floor(n/1) ) + a( floor(n/2) ) + ... + a(
 *          floor(n/n) ).
 * https://oeis.org/A003318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = 1 + sum_{k=1}^{n} a(floor(n/k))
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 456, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n == 1) return 1;
    
    if (n > memo_len) {
        seq_t *new_memo = (seq_t *)malloc(sizeof(seq_t) * n);
        if (memo != NULL) {
            for (idx_t i = 0; i < memo_len; i++) {
                new_memo[i] = memo[i];
            }
            free(memo);
        }
        for (idx_t i = memo_len; i < n; i++) {
            new_memo[i] = 0;
        }
        memo = new_memo;
        memo_len = n;
    }
    
    if (memo[n - 1] != 0) {
        return memo[n - 1];
    }
    
    seq_t result = 1;
    for (idx_t k = 1; k < n; k++) {
        idx_t idx = (n - 1) / k;
        result += a(idx);
    }
    
    memo[n - 1] = result;
    return result;
}
