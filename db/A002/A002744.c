/*
 * A002744  Sum of logarithmic numbers.
 * https://oeis.org/A002744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(n-k)*A000005(k)*(k-1)!*binomial(n, k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   compute_tau
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_tau.h"

static seq_t compute_binomial(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    
    if (k > n - k) {
        k = n - k;
    }
    
    seq_t result = 1;
    for (idx_t i = 0; i < k; i++) {
        result = result * (seq_t)(n - i) / (seq_t)(i + 1);
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t fact = 1;
    
    for (idx_t k = 1; k <= n; k++) {
        seq_t tau_k = compute_tau(k);
        seq_t binom = compute_binomial(n, k);
        
        int sign = ((n - k) & 1) ? -1 : 1;
        
        seq_t term = sign * tau_k * fact * binom;
        sum += term;
        
        fact *= (seq_t)k;
    }
    
    return sum;
}
