/*
 * A004065  Define predecessors of n, P(n), to consist of numbers whose binary
 *          representation is obtained from that of n by replacing 10 with 01
 *          or changing a final 1 to a 0; then a(0)=1, a(n) = Sum a(P(n)),
 *          n>0.
 * https://oeis.org/A004065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of a(p) for all predecessors p of n, a(0) = 1.
 *            Predecessors of n are obtained by: replacing any "10" with "01"
 *            in binary, or changing a final 1 to 0
 * Range:     u8 n <= 28, u16 n <= 89, u32 n <= 236, u64 n <= 988,
 *            u128 n <= 7674, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;
    
    if (memo == NULL) {
        memo_len = 1024;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memset(memo, 0, memo_len * sizeof(seq_t));
        memo[0] = 1;
    }
    
    if (n < memo_len && memo[n] != 0) {
        return memo[n];
    }
    
    if (n >= memo_len) {
        idx_t new_len = memo_len;
        while (new_len <= n) new_len *= 2;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        memcpy(new_memo, memo, memo_len * sizeof(seq_t));
        memset(new_memo + memo_len, 0, (new_len - memo_len) * sizeof(seq_t));
        free(memo);
        memo = new_memo;
        memo_len = new_len;
    }
    
    seq_t result = 0;
    
    for (int bit = 0; bit < 63; bit++) {
        if (((n >> bit) & 3) == 2) {
            idx_t pred = n ^ (3ULL << bit);
            result += a(pred);
        }
    }
    
    if (n & 1) {
        idx_t pred = n ^ 1;
        result += a(pred);
    }
    
    memo[n] = result;
    return result;
}
