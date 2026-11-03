/*
 * A003108  Number of partitions of n into cubes.
 * https://oeis.org/A003108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in 1/Product_{j>=1} (1-x^(j^3))
 * Range:     u8 n <= 215, u16 n <= 974, u32 n <= 5844, u64 n <= 45582,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 103 terms through n = 9804 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"


seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t *dp = mem_new(seq_t, ((size_t)n + 1));
    mem_set(dp, 0, ((size_t)n + 1) * sizeof(seq_t));
    dp[0] = 1;
    
    for (idx_t j = 1; ; j++) {
        idx_t cube = j * j * j;
        if (cube > n) break;
        
        for (idx_t i = cube; i <= n; i++) {
            dp[i] = dp[i] + dp[i - cube];
        }
    }
    
    seq_t result = dp[n];
    mem_free(dp);
    return result;
}
