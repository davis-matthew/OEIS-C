/*
 * A006119  Sum of Gaussian binomial coefficients [ n,k ] for q=5.
 * https://oeis.org/A006119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + (5^(n-1) - 1)*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/a000248_pow.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (memo == NULL) {
        memo_len = (n < 10) ? 10 : n + 1;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[0] = 1;
        computed_up_to = 0;
    }
    
    if (n >= memo_len) {
        idx_t new_len = memo_len;
        while (n >= new_len) new_len *= 2;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        memcpy(new_memo, memo, memo_len * sizeof(seq_t));
        free(memo);
        memo = new_memo;
        memo_len = new_len;
    }
    
    if (computed_up_to == 0) {
        memo[1] = 2;
        computed_up_to = 1;
    }
    
    if (n >= 2 && n > computed_up_to) {
        seq_t pow5 = a000248_pow((seq_t)5, computed_up_to);
        for (idx_t i = computed_up_to + 1; i <= n; i++) {
            memo[i] = 2 * memo[i - 1] + (pow5 - 1) * memo[i - 2];
            pow5 *= 5;
        }
        computed_up_to = n;
    }
    
    return memo[n];
}
