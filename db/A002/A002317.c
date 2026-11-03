/*
 * A002317  Related to Genocchi numbers.
 * https://oeis.org/A002317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) - 6*a(n-3) - a(n-4), with a(0)=2,
 *            a(1)=5, a(2)=7, a(3)=-26
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 16, i64 n <= 33, i128 n <= 66,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 995 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"


static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < 4) {
        if (n == 0) return 2;
        if (n == 1) return 5;
        if (n == 2) return 7;
        return -26;
    }
    
    if (memo == NULL) {
        memo_len = (n + 1 > 100) ? n + 1 : 100;
        memo = mem_new(seq_t, memo_len);
        memo[0] = 2;
        memo[1] = 5;
        memo[2] = 7;
        memo[3] = -26;
        for (idx_t i = 4; i < memo_len; i++) {
            memo[i] = 6 * memo[i-1] - 11 * memo[i-2] - 6 * memo[i-3] - memo[i-4];
        }
    } else if (n >= memo_len) {
        idx_t old_len = memo_len;
        memo_len = n + 1;
        memo = mem_resize(seq_t, memo, memo_len);
        for (idx_t i = old_len; i < memo_len; i++) {
            memo[i] = 6 * memo[i-1] - 11 * memo[i-2] - 6 * memo[i-3] - memo[i-4];
        }
    }
    
    return memo[n];
}
