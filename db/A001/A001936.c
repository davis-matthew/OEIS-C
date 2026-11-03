/*
 * A001936  Expansion of q^(-1/4) * (eta(q^4) / eta(q))^2 in powers of q.
 * https://oeis.org/A001936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 78, u64 n <= 263, u128 n <= 939,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 990 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Euler transform of period 4 sequence [2, 2, 2, 0, ...]. */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    seq_t *dp = mem_new(seq_t, (n + 1));
    
    dp[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        dp[i] = 0;
    }
    
    for (idx_t k = 1; k <= n; k++) {
        if (k % 4 == 0) continue;
        
        // Apply (1 - x^k)^{-1} twice to get (1 - x^k)^{-2}
        for (idx_t i = k; i <= n; i++) {
            dp[i] = dp[i] + dp[i - k];
        }
        
        for (idx_t i = k; i <= n; i++) {
            dp[i] = dp[i] + dp[i - k];
        }
    }
    
    seq_t result = dp[n];
    mem_free(dp);
    return result;
}
