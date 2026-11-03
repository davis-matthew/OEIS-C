/*
 * A001649  A Fielder sequence.
 * https://oeis.org/A001649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-6) for n >= 7,
 *            with a(1) = 1, a(2) = 3, a(3) = 7, a(4) = 15, a(5) = 26, a(6) =
 *            57
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 66, u128 n <= 133,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 983 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *memo = NULL;
static idx_t memo_cap = 0;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (memo == NULL) {
        memo_cap = 256;
        memo = mem_new(seq_t, memo_cap);
        memo[1] = 1;
        memo[2] = 3;
        memo[3] = 7;
        memo[4] = 15;
        memo[5] = 26;
        memo[6] = 57;
        memo_len = 6;
    }
    
    if (n <= memo_len) {
        return memo[n];
    }
    
    while (n >= memo_cap) {
        memo_cap *= 2;
        memo = mem_resize(seq_t, memo, memo_cap);
    }
    
    for (idx_t i = memo_len + 1; i <= n; i++) {
        memo[i] = memo[i-1] + memo[i-2] + memo[i-3] + memo[i-4] + memo[i-6];
    }
    memo_len = n;
    
    return memo[n];
}
