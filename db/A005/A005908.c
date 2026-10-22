/*
 * A005908  a(n) = floor( phi*a(n-1) ) + floor( phi*a(n-2) ), where phi is the
 *          golden ratio.
 * https://oeis.org/A005908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-4) - a(n-5); a(0)=0, a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=4
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 28, u64 n <= 55, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 2;
    if (n == 4) return 4;
    
    if (memo == NULL) {
        memo_len = n + 1;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[0] = 0;
        memo[1] = 1;
        memo[2] = 1;
        memo[3] = 2;
        memo[4] = 4;
        
        for (idx_t i = 5; i <= n; i++) {
            memo[i] = 2 * memo[i-1] + memo[i-2] - memo[i-4] - memo[i-5];
        }
    } else if (n >= memo_len) {
        seq_t *old_memo = memo;
        idx_t old_len = memo_len;
        memo_len = n + 1;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        
        for (idx_t i = 0; i < old_len; i++) {
            memo[i] = old_memo[i];
        }
        
        for (idx_t i = old_len; i <= n; i++) {
            memo[i] = 2 * memo[i-1] + memo[i-2] - memo[i-4] - memo[i-5];
        }
        
        free(old_memo);
    }
    
    return memo[n];
}
