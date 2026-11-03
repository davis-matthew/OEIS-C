/*
 * A006336  a(n) = a(n-1) + a(n - 1 - number of even terms so far).
 * https://oeis.org/A006336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + a([n/phi]), a(1) = 1, where phi = (1+sqrt(5))/2
 * Range:     u8 n <= 17, u16 n <= 62, u32 n <= 324, u64 n <= 3004,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9406 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;
    
    if (n < 1) return 0;
    
    if (memo == NULL) {
        memo_len = (n < 1000) ? 1000 : (n + 100);
        memo = mem_new(seq_t, memo_len);
        for (idx_t i = 0; i < memo_len; i++) {
            memo[i] = 0;
        }
    } else if (n >= memo_len) {
        idx_t new_len = n + 100;
        seq_t *new_memo = mem_new(seq_t, new_len);
        for (idx_t i = 0; i < memo_len; i++) {
            new_memo[i] = memo[i];
        }
        for (idx_t i = memo_len; i < new_len; i++) {
            new_memo[i] = 0;
        }
        mem_free(memo);
        memo = new_memo;
        memo_len = new_len;
    }
    
    double phi = (1.0 + sqrt(5.0)) / 2.0;
    for (idx_t i = 1; i <= n; i++) {
        if (memo[i - 1] == 0) {
            if (i == 1) {
                memo[0] = 1;
            } else {
                idx_t index = (idx_t)((double)i / phi);
                if (index < 1) index = 1;
                a(index);
                memo[i - 1] = memo[i - 2] + memo[index - 1];
            }
        }
    }
    
    return memo[n - 1];
}
