/*
 * A005709  a(n) = a(n-1) + a(n-7), with a(i) = 1 for i = 0..6.
 * https://oeis.org/A005709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-7), a(i) = 1 for i = 0..6
 * Range:     u8 n <= 27, u16 n <= 52, u32 n <= 101, u64 n <= 198,
 *            u128 n <= 393, big any n
 * Validated: standard, pass, 71 terms through n = 496 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < 7) {
        return 1;
    }
    
    if (memo != NULL && n < memo_len && memo[n] != 0) {
        return memo[n];
    }
    
    if (memo == NULL) {
        memo_len = 7;
        memo = mem_new(seq_t, 7);
        for (idx_t i = 0; i < 7; i++) {
            memo[i] = 1;
        }
    }
    
    if (n >= memo_len) {
        idx_t old_len = memo_len;
        memo_len = n + 1;
        memo = mem_resize(seq_t, memo, memo_len);
        mem_set(memo + old_len, 0, (memo_len - old_len) * sizeof(seq_t));
    }
    
    for (idx_t i = 7; i <= n; i++) {
        if (memo[i] == 0) {
            memo[i] = memo[i - 1] + memo[i - 7];
        }
    }
    
    return memo[n];
}
