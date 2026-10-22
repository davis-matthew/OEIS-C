/*
 * A007068  a(n) = a(n-1) + (3+(-1)^n)*a(n-2)/2.
 * https://oeis.org/A007068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + (3+(-1)^n)*a(n-2)/2, a(1) = 1, a(2) = 3
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t computed = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (n <= computed) {
        return memo[n - 1];
    }
    
    if (n > memo_len) {
        idx_t new_len = (memo_len == 0) ? 
            ((n > 100) ? n + 50 : 100) : 
            (memo_len * 2 < n ? n + 50 : memo_len * 2);
        
        seq_t *new_memo = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        if (new_memo == NULL) return 0;
        memo = new_memo;
        memo_len = new_len;
    }
    
    if (computed < 1) {
        memo[0] = 1;
        computed = 1;
    }
    if (computed < 2) {
        memo[1] = 3;
        computed = 2;
    }
    
    for (idx_t i = computed + 1; i <= n; i++) {
        if (i % 2 == 1) {
            memo[i - 1] = memo[i - 2] + memo[i - 3];
        } else {
            memo[i - 1] = memo[i - 2] + 2 * memo[i - 3];
        }
    }
    computed = n;
    
    return memo[n - 1];
}
