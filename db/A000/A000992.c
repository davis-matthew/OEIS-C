/*
 * A000992  "Half-Catalan numbers": a(n) = Sum_{k=1..floor(n/2)} a(k)*a(n-k)
 *          with a(1) = 1.
 * https://oeis.org/A000992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} a(k)*a(n-k), a(1) = 1
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 31, u64 n <= 56, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (n >= memo_len) {
        idx_t new_len = (n + 1) * 2;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (new_memo == NULL) return 0;
        
        if (memo != NULL) {
            for (idx_t i = 0; i < memo_len; i++) {
                new_memo[i] = memo[i];
            }
            free(memo);
        }
        
        for (idx_t i = memo_len; i < new_len; i++) {
            new_memo[i] = 0;
        }
        
        memo = new_memo;
        memo_len = new_len;
    }
    
    if (memo[n] != 0) {
        return memo[n];
    }
    
    if (n == 1) {
        memo[n] = 1;
        return 1;
    }
    
    seq_t result = 0;
    idx_t half_n = n / 2;
    
    for (idx_t k = 1; k <= half_n; k++) {
        result = result + a(k) * a(n - k);
    }
    
    memo[n] = result;
    return result;
}
