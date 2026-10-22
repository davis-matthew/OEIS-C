/*
 * A002045  MacMahon's solid partitions of n in which 4 is the smallest
 *          summand.
 * https://oeis.org/A002045
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{i>=4} (-1 * [i=4] + 1/(1 - x^i)^binomial(i+1,2))
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t binom(seq_t n, idx_t k) {
    if (k > (idx_t)n || k < 0) return 0;
    if (k == 0 || (idx_t)n == k) return 1;
    
    seq_t result = 1;
    for (idx_t i = 0; i < k; i++) {
        result = result * (n - (seq_t)i) / (seq_t)(i + 1);
    }
    return result;
}

seq_t compute_partitions(idx_t n, idx_t min_part) {
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= n; i++) dp[i] = 0;
    dp[0] = 1;
    
    for (idx_t part = min_part; part <= n; part++) {
        seq_t exp = (seq_t)part * ((seq_t)part + 1) / 2;
        
        seq_t *new_dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= n; i++) new_dp[i] = 0;
        
        for (idx_t i = 0; i <= n; i++) {
            for (idx_t m = 0; m * part <= i; m++) {
                seq_t binom_coeff = binom(exp + (seq_t)m - 1, m);
                new_dp[i] += binom_coeff * dp[i - m * part];
            }
        }
        
        free(dp);
        dp = new_dp;
    }
    
    seq_t result = dp[n];
    free(dp);
    return result;
}

seq_t a(idx_t n) {
    if (n < 4) return 0;
    
    seq_t f4 = compute_partitions(n, 4);
    seq_t f5 = compute_partitions(n, 5);
    
    return f4 - f5;
}
