/*
 * A005386  Area of n-th triple of squares around a triangle.
 * https://oeis.org/A005386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2) - a(n-3), a(1)=1, a(2)=3, a(3)=16
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 997 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t memo_computed = 0;

seq_t a(idx_t n) {
    if (n <= 3) {
        if (n == 1) return 1;
        if (n == 2) return 3;
        return 16;
    }
    
    if (memo == NULL) {
        memo_len = n + 100;
        memo = mem_new(seq_t, memo_len);
        memo[1] = 1;
        memo[2] = 3;
        memo[3] = 16;
        memo_computed = 3;
    } else if (n >= memo_len) {
        seq_t *new_memo = mem_new(seq_t, (n + 100));
        mem_copy(new_memo, memo, memo_len * sizeof(seq_t));
        mem_free(memo);
        memo = new_memo;
        memo_len = n + 100;
    }
    
    for (idx_t i = memo_computed + 1; i <= n; i++) {
        memo[i] = 4*memo[i-1] + 4*memo[i-2] - memo[i-3];
    }
    if (n > memo_computed) {
        memo_computed = n;
    }
    
    return memo[n];
}
