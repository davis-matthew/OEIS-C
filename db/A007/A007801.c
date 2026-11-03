/*
 * A007801  Expansion of f(f(x)), where f = x + x^2 + x^4 + x^8 + x^16 + ...
 * https://oeis.org/A007801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] f(f(x)), where f(x) = x + x^2 + x^4 + x^8 +
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 45, u64 n <= 88, u128 n <= 174,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 1019 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    
    seq_t *dp = mem_new(seq_t, (size_t)n + 1);
    if (dp == NULL) return 0;
    
    // Initialize dp[i] = [x^i] f(x)
    for (int k = 0; (1ULL << k) <= n; k++) {
        idx_t pow = 1ULL << k;
        dp[pow] = 1;
    }
    
    // Add [x^n] f(x) to result
    result += dp[n];
    
    seq_t *new_dp = mem_new(seq_t, (size_t)n + 1);
    if (new_dp == NULL) {
        mem_free(dp);
        return result;
    }
    
    // Compute f(x)^2, f(x)^4, ... and add coefficients
    for (int k = 1; (1ULL << k) <= n; k++) {
        mem_set(new_dp, 0, ((size_t)n + 1) * sizeof(seq_t));
        
        for (idx_t i = 1; i <= n; i++) {
            for (idx_t j = 1; i + j <= n; j++) {
                new_dp[i + j] += dp[i] * dp[j];
            }
        }
        
        mem_copy(dp, new_dp, ((size_t)n + 1) * sizeof(seq_t));
        result += dp[n];
    }
    
    mem_free(dp);
    mem_free(new_dp);
    
    return result;
}
