/*
 * A005327  Certain subgraphs of a directed graph (inverse binomial transform
 *          of A005321).
 * https://oeis.org/A005327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(n-1)-1)*a(n-1) + (-1)^(n-1) for n > 1, with a(1) = 1,
 *            a(2) = 0
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 0;
    
    // Ensure memo array is large enough
    if (n >= memo_len) {
        idx_t new_len = n + 100;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (memo) {
            memcpy(new_memo, memo, memo_len * sizeof(seq_t));
            free(memo);
        }
        memset(new_memo + memo_len, 0, (new_len - memo_len) * sizeof(seq_t));
        memo = new_memo;
        memo_len = new_len;
    }
    
    // Initialize base cases if needed
    if (max_computed < 2) {
        memo[1] = 1;
        memo[2] = 0;
        max_computed = 2;
    }
    
    // Compute values from max_computed+1 to n
    for (idx_t i = max_computed + 1; i <= n; i++) {
        seq_t coeff = (((seq_t)1) << (i - 1)) - 1;
        seq_t sign_term = ((i % 2) == 1) ? 1 : -1;
        memo[i] = coeff * memo[i - 1] + sign_term;
    }
    
    max_computed = (n > max_computed) ? n : max_computed;
    
    return memo[n];
}
