/*
 * A004149  Generalized Catalan numbers: a(n+1) = a(n) + Sum_{k=2..n-1}
 *          a(k)*a(n-1-k).
 * https://oeis.org/A004149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n+1) = a(n) + Sum_{k=2..n-1} a(k)*a(n-1-k), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 31, u64 n <= 57, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (memo_len <= n) {
        idx_t new_len = n + 1;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        idx_t start_i;
        
        if (memo == NULL) {
            new_memo[0] = 1;
            if (new_len > 1) new_memo[1] = 1;
            start_i = 2;
        } else {
            for (idx_t i = 0; i < memo_len; i++) {
                new_memo[i] = memo[i];
            }
            free(memo);
            start_i = memo_len;
        }
        
        for (idx_t i = start_i; i < new_len; i++) {
            seq_t result = new_memo[i - 1];
            for (idx_t k = 2; k < i - 1; k++) {
                result = result + new_memo[k] * new_memo[i - 2 - k];
            }
            new_memo[i] = result;
        }
        
        memo = new_memo;
        memo_len = new_len;
    }
    
    return memo[n];
}
