/*
 * A006051  Square hex numbers.
 * https://oeis.org/A006051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 194*a(n-1) - a(n-2) - 24, a(1) = 1, a(2) = 169
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
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
    if (n < 1) return 0;
    
    if (memo == NULL) {
        memo_len = (n < 100) ? 100 : n + 10;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[0] = 1;
        memo[1] = 169;
        computed_up_to = 2;
    }
    
    if (n <= computed_up_to) {
        return memo[n - 1];
    }
    
    if (n > memo_len) {
        idx_t old_len = memo_len;
        while (memo_len < n) {
            memo_len = memo_len * 2;
        }
        seq_t *new_memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        for (idx_t i = 0; i < old_len; i++) {
            new_memo[i] = memo[i];
        }
        free(memo);
        memo = new_memo;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        memo[i - 1] = 194 * memo[i - 2] - memo[i - 3] - 24;
    }
    computed_up_to = n;
    
    return memo[n - 1];
}
