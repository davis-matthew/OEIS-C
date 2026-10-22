/*
 * A005825  Numerators in a worst case of a Jacobi symbol algorithm.
 * https://oeis.org/A005825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-3) + 4*a(n-4), a(0) = 0, a(1) = 1, a(2)
 *            = 7, a(3) = 31
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 59,
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
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 7;
    if (n == 3) return 31;
    
    if (n < computed_up_to) {
        return memo[n];
    }
    
    if (memo == NULL) {
        memo_len = n + 1;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[0] = 0;
        memo[1] = 1;
        memo[2] = 7;
        memo[3] = 31;
        computed_up_to = 4;
    }
    
    while (memo_len <= n) {
        memo_len *= 2;
        seq_t *new_memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        for (idx_t i = 0; i < computed_up_to; i++) {
            new_memo[i] = memo[i];
        }
        free(memo);
        memo = new_memo;
    }
    
    for (idx_t i = computed_up_to; i <= n; i++) {
        memo[i] = 5 * memo[i-1] - 10 * memo[i-3] + 4 * memo[i-4];
    }
    
    computed_up_to = n + 1;
    
    return memo[n];
}
