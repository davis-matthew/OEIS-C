/*
 * A006328  Total preorders.
 * https://oeis.org/A006328
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 4*a(n-3) + 4*a(n-4) + a(n-5) -
 *            a(n-6) for n>8 Base cases: a(3)=5, a(4)=24, a(5)=79, a(6)=223,
 *            a(7)=579, a(8)=1432
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 53, u128 n <= 108,
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
    if (n < 3) return 0;
    
    if (memo_len <= n) {
        idx_t new_len = n + 256;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (new_memo == NULL) return 0;
        
        if (memo != NULL) {
            memcpy(new_memo, memo, memo_len * sizeof(seq_t));
            free(memo);
        }
        
        memset(new_memo + memo_len, 0, (new_len - memo_len) * sizeof(seq_t));
        memo = new_memo;
        memo_len = new_len;
    }
    
    for (idx_t i = 3; i <= n; ++i) {
        if (memo[i] == 0) {
            if (i == 3) memo[i] = 5;
            else if (i == 4) memo[i] = 24;
            else if (i == 5) memo[i] = 79;
            else if (i == 6) memo[i] = 223;
            else if (i == 7) memo[i] = 579;
            else if (i == 8) memo[i] = 1432;
            else {
                memo[i] = ((seq_t)4 * memo[i-1]) - ((seq_t)3 * memo[i-2]) - ((seq_t)4 * memo[i-3])
                        + ((seq_t)4 * memo[i-4]) + memo[i-5] - memo[i-6];
            }
        }
    }
    
    return memo[n];
}
