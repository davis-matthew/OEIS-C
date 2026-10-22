/*
 * A005323  Column of Motzkin triangle.
 * https://oeis.org/A005323
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n + 5)(n - 3) a(n) = n(2n + 1) a(n - 1) + 3n(n - 1) a(n - 2),
 *            a(3) = 1, a(4) = 4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 28, u128 n <= 28,
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
    if (n == 3) return 1;
    if (n == 4) return 4;
    
    if (memo_len <= n) {
        idx_t new_len = n + 1;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (new_memo == NULL) return 0;
        
        memset(new_memo, 0, new_len * sizeof(seq_t));
        
        if (memo != NULL) {
            memcpy(new_memo, memo, memo_len * sizeof(seq_t));
            free(memo);
        }
        
        new_memo[3] = 1;
        if (new_len > 4) new_memo[4] = 4;
        
        memo = new_memo;
        memo_len = new_len;
    }
    
    for (idx_t i = 5; i <= n; i++) {
        if (memo[i] != 0) continue;
        
        seq_t numerator = (seq_t)i * (seq_t)(2*i + 1) * memo[i-1] + 
                          (seq_t)3 * (seq_t)i * (seq_t)(i - 1) * memo[i-2];
        seq_t denominator = (seq_t)(i + 5) * (seq_t)(i - 3);
        
        memo[i] = numerator / denominator;
    }
    
    return memo[n];
}
