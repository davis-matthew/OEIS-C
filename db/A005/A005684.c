/*
 * A005684  Number of Twopins positions.
 * https://oeis.org/A005684
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 3*a(n-4) - 2*a(n-5) - a(n-8), with
 *            a(6) = 1, a(7) = 2, a(8) = 4, a(9) = 6, a(10) = 11, a(11) = 18,
 *            a(12) = 32, a(13) = 52
 * Range:     u8 n <= 16, u16 n <= 27, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < 6) return 0;
    
    if (memo == NULL) {
        memo_len = 14;
        memo = (seq_t *)calloc(memo_len, sizeof(seq_t));
        memo[6] = 1;
        memo[7] = 2;
        memo[8] = 4;
        memo[9] = 6;
        memo[10] = 11;
        memo[11] = 18;
        memo[12] = 32;
        memo[13] = 52;
    }
    
    if (memo_len <= n) {
        idx_t new_len = (n + 1) * 2;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        memcpy(new_memo, memo, memo_len * sizeof(seq_t));
        memset(new_memo + memo_len, 0, (new_len - memo_len) * sizeof(seq_t));
        free(memo);
        memo = new_memo;
        memo_len = new_len;
    }
    
    for (idx_t i = 14; i <= n; i++) {
        if (memo[i] == 0) {
            memo[i] = 2 * memo[i-1] - 2 * memo[i-3] + 3 * memo[i-4] - 2 * memo[i-5] - memo[i-8];
        }
    }
    
    return memo[n];
}
