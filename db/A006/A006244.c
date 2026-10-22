/*
 * A006244  Hexagonal numbers (A000384) which are also centered hexagonal
 *          numbers (A003215).
 * https://oeis.org/A006244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 98*a(n-1) - a(n-2) - 6, a(1) = 1, a(2) = 91
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
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
    if (n == 1) return 1;
    if (n == 2) return 91;
    
    if (memo == NULL) {
        memo_len = 16;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[1] = 1;
        memo[2] = 91;
        computed_up_to = 2;
    }
    
    while (n >= memo_len) {
        idx_t new_len = memo_len * 2;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < memo_len; i++) {
            new_memo[i] = memo[i];
        }
        free(memo);
        memo = new_memo;
        memo_len = new_len;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        memo[i] = 98 * memo[i-1] - memo[i-2] - 6;
    }
    if (n > computed_up_to) {
        computed_up_to = n;
    }
    
    return memo[n];
}
