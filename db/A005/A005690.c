/*
 * A005690  Number of Twopins positions.
 * https://oeis.org/A005690
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) + 2*a(n-6) - 2*a(n-7) -
 *            a(n-12) for n >= 9, with a(8) = 1, a(n) = 0 for n < 8
 * Range:     u8 n <= 20, u16 n <= 34, u32 n <= 63, u64 n <= 121,
 *            u128 n <= 238, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 8) return 0;
    if (n == 8) return 1;
    
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;
    
    if (memo == NULL) {
        memo_len = n + 1;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        
        for (idx_t i = 0; i < 8; i++) {
            memo[i] = 0;
        }
        memo[8] = 1;
        
        for (idx_t i = 9; i <= n; i++) {
            seq_t val = (seq_t)2 * memo[i - 1];
            val -= (seq_t)2 * memo[i - 3];
            val += memo[i - 4];
            val += (seq_t)2 * memo[i - 6];
            val -= (seq_t)2 * memo[i - 7];
            if (i >= 12) val -= memo[i - 12];
            memo[i] = val;
        }
    } else if (n >= memo_len) {
        idx_t old_len = memo_len;
        memo_len = n + 1;
        memo = (seq_t *)realloc(memo, memo_len * sizeof(seq_t));
        
        for (idx_t i = old_len; i <= n; i++) {
            seq_t val = (seq_t)2 * memo[i - 1];
            val -= (seq_t)2 * memo[i - 3];
            val += memo[i - 4];
            val += (seq_t)2 * memo[i - 6];
            val -= (seq_t)2 * memo[i - 7];
            if (i >= 12) val -= memo[i - 12];
            memo[i] = val;
        }
    }
    
    return memo[n];
}
