/*
 * A006183  a(n) = (n+1)*a(n-1) + (2-n)*a(n-2).
 * https://oeis.org/A006183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) - (n-3)*a(n-2), a(1) = 1, a(2) = 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;
    
    if (memo == NULL) {
        memo_len = (n < 1000) ? 1000 : n + 100;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        if (memo == NULL) return 0;
        memset(memo, 0, memo_len * sizeof(seq_t));
        memo[1] = 1;
        memo[2] = 2;
        computed_up_to = 2;
    }
    
    while (n >= memo_len) {
        idx_t new_len = memo_len * 2;
        seq_t *new_memo = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        if (new_memo == NULL) return 0;
        memset(&new_memo[memo_len], 0, (new_len - memo_len) * sizeof(seq_t));
        memo = new_memo;
        memo_len = new_len;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t term1 = ((seq_t)i) * memo[i - 1];
        seq_t term2 = 0;
        if (i > 3) {
            term2 = ((seq_t)(i - 3)) * memo[i - 2];
        }
        memo[i] = term1 - term2;
    }
    
    if (n > computed_up_to) {
        computed_up_to = n;
    }
    
    return memo[n];
}
