/*
 * A001907  Expansion of e.g.f. exp(-x)/(1-4*x).
 * https://oeis.org/A001907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*n-1)*a(n-1) + 4*(n-1)*a(n-2), a(0) = 1, a(1) = 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 24,
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
    
    if (memo == NULL) {
        memo_len = 128;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[0] = 1;
        memo[1] = 3;
        memo_computed = 2;
    }
    
    if (n >= memo_len) {
        idx_t new_len = memo_len;
        while (new_len <= n) new_len *= 2;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < memo_len; i++) {
            new_memo[i] = memo[i];
        }
        free(memo);
        memo = new_memo;
        memo_len = new_len;
    }
    
    while (memo_computed <= n) {
        seq_t i = (seq_t)memo_computed;
        seq_t coeff1 = 4 * i - 1;
        seq_t coeff2 = 4 * (i - 1);
        memo[memo_computed] = coeff1 * memo[memo_computed - 1] + coeff2 * memo[memo_computed - 2];
        memo_computed++;
    }
    
    return memo[n];
}
