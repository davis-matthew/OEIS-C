/*
 * A000929  Dimension of the n-th graded piece of the mod-2 Steenrod algebra
 *          A_2.
 * https://oeis.org/A000929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 280, u32 n <= 2074, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 10000 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partition n into Mersenne numbers {1, 3, 7, 15, ...} using dynamic programming */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    seq_t *dp = mem_new(seq_t, (n + 1));
    if (!dp) return 0;
    
    mem_set(dp, 0, (n + 1) * sizeof(seq_t));
    dp[0] = 1;
    
    for (idx_t mersenne = 1; mersenne <= n; mersenne = 2 * mersenne + 1) {
        for (idx_t i = mersenne; i <= n; i++) {
            dp[i] += dp[i - mersenne];
        }
    }
    
    seq_t result = dp[n];
    mem_free(dp);
    return result;
}
