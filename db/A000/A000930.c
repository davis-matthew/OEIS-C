/*
 * A000930  Narayana's cows sequence: a(0) = a(1) = a(2) = 1; thereafter a(n)
 *          = a(n-1) + a(n-3).
 * https://oeis.org/A000930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = a(n-1) + a(n-3), a(0) = 1, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 59, u64 n <= 117,
 *            u128 n <= 233, big any n
 * Validated: standard, pass, 60 terms through n = 4807 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *memo = NULL;
static idx_t memo_cap = 0;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n <= 2) return 1;
    
    if (n >= memo_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_memo = mem_new(seq_t, new_cap);
        if (memo != NULL) {
            mem_copy(new_memo, memo, memo_len * sizeof(seq_t));
            mem_free(memo);
        }
        memo = new_memo;
        memo_cap = new_cap;
    }
    
    if (memo_len == 0) {
        memo[0] = 1;
        memo[1] = 1;
        memo[2] = 1;
        memo_len = 3;
    }
    
    for (idx_t i = memo_len; i <= n; i++) {
        memo[i] = memo[i-1] + memo[i-3];
    }
    if (n + 1 > memo_len) memo_len = n + 1;
    
    return memo[n];
}
