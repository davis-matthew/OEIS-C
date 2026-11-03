/*
 * A005206  Hofstadter G-sequence: a(0) = 0; a(n) = n - a(a(n-1)) for n > 0.
 * https://oeis.org/A005206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 0; a(n) = n - a(a(n-1)) for n > 0
 * Range:     u8 n <= 413, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 93 terms through n = 8332 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *memo = NULL;
static idx_t memo_size = 0;
static idx_t memo_computed = 0;

seq_t a(idx_t n) {
    if (n < memo_computed) {
        return memo[n];
    }
    
    if (memo_size == 0) {
        memo_size = 1000;
        memo = mem_new(seq_t, memo_size);
    }
    
    while (n >= memo_size) {
        idx_t new_size = memo_size * 2;
        seq_t *new_memo = mem_new(seq_t, new_size);
        for (idx_t i = 0; i < memo_computed; i++) {
            new_memo[i] = memo[i];
        }
        mem_free(memo);
        memo = new_memo;
        memo_size = new_size;
    }
    
    for (idx_t i = memo_computed; i <= n; i++) {
        if (i == 0) {
            memo[i] = 0;
        } else {
            seq_t prev_val = memo[i - 1];
            idx_t prev_idx = (idx_t)prev_val;
            memo[i] = (seq_t)i - memo[prev_idx];
        }
    }
    memo_computed = n + 1;
    
    return memo[n];
}
