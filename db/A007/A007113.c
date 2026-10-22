/*
 * A007113  Expansion of e.g.f. (1 + x)^x.
 * https://oeis.org/A007113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n, k)*k!*Stirling1(n-k, k)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    // Precompute Stirling numbers of the first kind using recurrence:
    // S(i,j) = S(i-1,j-1) - (i-1)*S(i-1,j)
    seq_t **stir = (seq_t **)malloc((n + 1) * sizeof(seq_t *));
    for (idx_t i = 0; i <= n; i++) {
        stir[i] = (seq_t *)calloc(n + 1, sizeof(seq_t));
    }
    stir[0][0] = 1;
    
    for (idx_t i = 1; i <= n; i++) {
        for (idx_t j = 1; j <= i; j++) {
            stir[i][j] = stir[i-1][j-1] - (seq_t)(i-1) * stir[i-1][j];
        }
    }
    
    // Precompute factorials
    seq_t *fact = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    fact[0] = 1;
    for (idx_t k = 1; k <= n; k++) {
        fact[k] = fact[k-1] * (seq_t)k;
    }
    
    // Compute the result
    seq_t result = 0;
    idx_t limit = n / 2;
    for (idx_t k = 0; k <= limit; k++) {
        seq_t term = binom(n, k) * fact[k] * stir[n - k][k];
        result += term;
    }
    
    // Free memory
    for (idx_t i = 0; i <= n; i++) {
        free(stir[i]);
    }
    free(stir);
    free(fact);
    
    return result;
}
