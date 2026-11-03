/*
 * A001909  a(n) = n*a(n-1) + (n-4)*a(n-2), a(2) = 0, a(3) = 1.
 * https://oeis.org/A001909
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-4)*a(n-2), a(2) = 0, a(3) = 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 100 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *memo = NULL;
static idx_t memo_cap = 0;
static idx_t highest_computed = 1;

seq_t a(idx_t n) {
    if (n < 2) return 0;
    
    if (memo == NULL) {
        memo_cap = 64;
        memo = mem_new(seq_t, memo_cap);
        for (idx_t i = 0; i < memo_cap; i++) {
            memo[i] = 0;
        }
        memo[2] = 0;
        memo[3] = 1;
        highest_computed = 3;
    }
    
    if (n > highest_computed) {
        if (n >= memo_cap) {
            idx_t newcap = memo_cap;
            while (newcap <= n) {
                newcap *= 2;
            }
            seq_t *newmemo = mem_new(seq_t, newcap);
            for (idx_t i = 0; i < memo_cap; i++) {
                newmemo[i] = memo[i];
            }
            for (idx_t i = memo_cap; i < newcap; i++) {
                newmemo[i] = 0;
            }
            mem_free(memo);
            memo = newmemo;
            memo_cap = newcap;
        }
        
        for (idx_t i = highest_computed + 1; i <= n; i++) {
            seq_t ci = (seq_t)i;
            seq_t ci_minus_4 = (seq_t)(i - 4);
            seq_t term1 = ci * memo[i-1];
            seq_t term2 = ci_minus_4 * memo[i-2];
            memo[i] = term1 + term2;
        }
        highest_computed = n;
    }
    
    return memo[n];
}
