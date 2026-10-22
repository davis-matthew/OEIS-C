/*
 * A006238  Number of spanning trees in the 3 X n grid graph.
 * https://oeis.org/A006238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15a(n-1) - 32a(n-2) + 15a(n-3) - a(n-4), n>4, with
 *            a(1)=1, a(2)=15, a(3)=192, a(4)=2415
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t last_computed = 0;

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 15;
    if (n == 3) return 192;
    if (n == 4) return 2415;
    
    if (memo == NULL) {
        memo_len = (n < 1000) ? 1000 : (n * 2);
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[1] = 1;
        memo[2] = 15;
        memo[3] = 192;
        memo[4] = 2415;
        last_computed = 4;
    }
    
    if (n >= memo_len) {
        idx_t new_len = n * 2;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < memo_len; i++) {
            new_memo[i] = memo[i];
        }
        free(memo);
        memo = new_memo;
        memo_len = new_len;
    }
    
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; i++) {
            seq_t val = 15 * memo[i - 1] - 32 * memo[i - 2] + 15 * memo[i - 3] - memo[i - 4];
            memo[i] = val;
        }
        last_computed = n;
    }
    
    return memo[n];
}
