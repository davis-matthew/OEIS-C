/*
 * A001569  Sum_{n>=0} a(n)*x^n/n!^2 = BesselI(0,2*(1-exp(x))^(1/2)).
 * https://oeis.org/A001569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n!*Sum_{k=0..n} (-1)^k*Stirling2(n,k)/k!
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    // Compute Stirling numbers S(n, k) for k=0..n using DP
    // S(n,k) = k*S(n-1,k) + S(n-1,k-1), with S(0,0)=1
    seq_t *prev = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
    
    memset(prev, 0, ((size_t)n + 1) * sizeof(seq_t));
    prev[0] = 1;  // S(0, 0) = 1
    
    // Build Stirling rows 1 through n
    for (idx_t i = 1; i <= n; i++) {
        memset(curr, 0, ((size_t)n + 1) * sizeof(seq_t));
        for (idx_t j = 1; j <= i; j++) {
            curr[j] = (seq_t)j * prev[j] + prev[j - 1];
        }
        // Swap
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }
    
    // Now prev contains S(n, k) for k = 0..n
    
    // Compute n!
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; i++) {
        fact_n *= (seq_t)i;
    }
    
    // Compute a(n) = Sum_{k=0..n} (-1)^k * (n!/k!) * S(n,k)
    seq_t result = 0;
    seq_t fact_k = 1;  // k!
    
    for (idx_t k = 0; k <= n; k++) {
        seq_t sign = (k & 1) ? -1 : 1;
        result += sign * (fact_n / fact_k) * prev[k];
        
        if (k < n) {
            fact_k *= (seq_t)(k + 1);
        }
    }
    
    free(prev);
    free(curr);
    return result;
}
