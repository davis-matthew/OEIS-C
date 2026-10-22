/*
 * A003749  Number of Hamiltonian cycles in K_5 X P_n.
 * https://oeis.org/A003749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-1) + 12*a(n-2), a(1) = 12, a(2) = 480, a(3) =
 *            13440
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t memo_computed = 3;

seq_t a(idx_t n) {
    if (n == 1) return 12;
    if (n == 2) return 480;
    if (n == 3) return 13440;
    
    if (memo == NULL) {
        memo_len = (n < 1000) ? 1000 : n + 100;
        memo = (seq_t *)calloc(memo_len, sizeof(seq_t));
        memo[1] = 12;
        memo[2] = 480;
        memo[3] = 13440;
    } else if (n >= memo_len) {
        idx_t old_len = memo_len;
        memo_len = n + 100;
        seq_t *new_memo = (seq_t *)calloc(memo_len, sizeof(seq_t));
        memcpy(new_memo, memo, old_len * sizeof(seq_t));
        free(memo);
        memo = new_memo;
    }
    
    if (n > memo_computed) {
        for (idx_t i = memo_computed + 1; i <= n; i++) {
            memo[i] = 28 * memo[i - 1] + 12 * memo[i - 2];
        }
        memo_computed = n;
    }
    
    return memo[n];
}
