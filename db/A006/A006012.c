/*
 * A006012  a(0) = 1, a(1) = 2, a(n) = 4*a(n-1) - 2*a(n-2), n >= 2.
 * https://oeis.org/A006012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_arr.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    
    if (n >= memo_len) {
        idx_t old_len = memo_len;
        memo_len = n + 1;
        memo = grow_arr(memo, old_len, memo_len);
        
        if (old_len == 0) {
            memo[0] = 1;
            memo[1] = 2;
            old_len = 2;
        }
        
        for (idx_t i = old_len; i <= n; i++) {
            memo[i] = 4 * memo[i-1] - 2 * memo[i-2];
        }
    }
    
    return memo[n];
}
