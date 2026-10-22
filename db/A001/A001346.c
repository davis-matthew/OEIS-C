/*
 * A001346  a(n) = Sum_{k = 0..4} (n+k)! C(4,k).
 * https://oeis.org/A001346
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..4} (n+k)! * C(4,k)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 0) {
        if (n == -1) return (seq_t)23;
        return 0;
    }
    
    seq_t result = 0;
    seq_t fact = 1;
    
    // Compute n!
    for (idx_t i = 1; i <= n; i++) {
        fact *= (seq_t)i;
    }
    
    // Binomial coefficients C(4,k)
    int binom[5] = {1, 4, 6, 4, 1};
    
    // Compute Sum_{k=0}^{4} (n+k)! * C(4,k)
    seq_t term_fact = fact;
    for (int k = 0; k <= 4; k++) {
        result += term_fact * (seq_t)binom[k];
        if (k < 4) {
            term_fact *= (seq_t)(n + k + 1);
        }
    }
    
    return result;
}
