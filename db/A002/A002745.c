/*
 * A002745  Sum of logarithmic numbers.
 * https://oeis.org/A002745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A000203(k)*(k-1)!*binomial(n, k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t fact = 1;  // (k-1)!
    seq_t binom = (seq_t)n;  // C(n, k)
    
    for (idx_t k = 1; k <= n; k++) {
        // Compute σ(k): sum of divisors
        seq_t sigma_k = 0;
        for (idx_t d = 1; d * d <= k; d++) {
            if (k % d == 0) {
                sigma_k += (seq_t)d;
                if (d * d != k) {
                    sigma_k += (seq_t)(k / d);
                }
            }
        }
        
        result += sigma_k * fact * binom;
        
        // Update for next iteration: fact = (k-1)! -> k!, binom = C(n,k) -> C(n,k+1)
        fact *= (seq_t)k;
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
    }
    
    return result;
}
