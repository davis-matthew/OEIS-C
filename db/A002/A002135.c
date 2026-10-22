/*
 * A002135  Number of terms in a symmetrical determinant: a(n) = n*a(n-1) -
 *          (n-1)*(n-2)*a(n-3)/2.
 * https://oeis.org/A002135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n*a(n-1) - (n-1)*(n-2)*a(n-3)/2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    // Initialize on first call
    if (memo == NULL) {
        memo = (seq_t *)malloc(sizeof(seq_t) * 3);
        memo[0] = 1;
        memo[1] = 1;
        memo[2] = 2;
        memo_len = 3;
        computed_up_to = 2;
    }
    
    // Grow memo array if needed
    if (n >= memo_len) {
        idx_t new_len = n + 1;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < memo_len; i++) {
            new_memo[i] = memo[i];
        }
        free(memo);
        memo = new_memo;
        memo_len = new_len;
    }
    
    // Compute values up to n if not already computed
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t i_s = (seq_t)i;
        seq_t term1 = i_s * memo[i-1];
        
        // Compute binomial(i-1, 2) = (i-1)*(i-2)/2
        // Avoid overflow: divide the even factor first
        seq_t binomial_coeff;
        if ((i & 1) == 0) {
            binomial_coeff = (seq_t)(i - 1) * (((seq_t)(i - 2)) / 2);
        } else {
            binomial_coeff = (((seq_t)(i - 1)) / 2) * (seq_t)(i - 2);
        }
        
        seq_t term2 = binomial_coeff * memo[i-3];
        memo[i] = term1 - term2;
    }
    if (n > computed_up_to) {
        computed_up_to = n;
    }
    
    return memo[n];
}
