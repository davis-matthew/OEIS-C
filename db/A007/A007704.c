/*
 * A007704  a(n+2) = (a(n) - 1)*a(n+1) + 1, with (a(1), a(2)) = (2, 3).
 * https://oeis.org/A007704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = (a(n) - 1)*a(n+1) + 1, with a(1) = 2, a(2) = 3
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 18 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *memo = NULL;
static idx_t memo_capacity = 0;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    // Ensure memo is large enough
    if (memo_capacity <= n) {
        idx_t new_capacity = (n > memo_capacity * 2) ? n + 10 : memo_capacity * 2 + 10;
        memo = mem_resize(seq_t, memo, (new_capacity + 1));
        memo_capacity = new_capacity;
    }
    
    // Initialize base cases if needed
    if (memo_len == 0) {
        memo[1] = 2;
        memo[2] = 3;
        memo_len = 2;
    }
    
    // Compute values up to n
    for (idx_t i = memo_len + 1; i <= n; i++) {
        seq_t val = (memo[i - 2] - 1) * memo[i - 1] + 1;
        memo[i] = val;
    }
    memo_len = (n > memo_len) ? n : memo_len;
    
    return memo[n];
}
