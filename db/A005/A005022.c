/*
 * A005022  Number of walks of length 2n+6 in the path graph P_7 from one end
 *          to the other.
 * https://oeis.org/A005022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) + 4*a(n-3), a(0) = 1, a(1) = 6, a(2)
 *            = 26
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)26;
    
    if (memo == NULL) {
        memo_len = 16;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)6;
        memo[2] = (seq_t)26;
    }
    
    if (memo_len <= n) {
        idx_t new_len = (n + 1) * 2;
        seq_t *tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < memo_len; i++) {
            tmp[i] = memo[i];
        }
        free(memo);
        memo = tmp;
        memo_len = new_len;
    }
    
    for (idx_t i = 3; i <= n; i++) {
        if (memo[i] == 0) {
            memo[i] = (seq_t)6 * memo[i-1] - (seq_t)10 * memo[i-2] + (seq_t)4 * memo[i-3];
        }
    }
    
    return memo[n];
}
