/*
 * A002771  Number of terms in a skew determinant: a(n) = (A000085(n) +
 *          A081919(n))/2.
 * https://oeis.org/A002771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n, 2*k) * (2*k-1)!! * (1 +
 *            (2*k-1)!!) / 2
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    
    seq_t result = 1;
    for (idx_t i = 0; i < k; i++) {
        result = result * ((seq_t)n - (seq_t)i) / ((seq_t)i + 1);
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t double_fact = 1;
    
    for (idx_t k = 0; k <= n / 2; k++) {
        seq_t binom_coeff = binom(n, 2*k);
        seq_t inner = double_fact * (1 + double_fact) / 2;
        seq_t term = binom_coeff * inner;
        result += term;
        
        if (k < n / 2) {
            double_fact *= (2*k + 1);
        }
    }
    
    return result;
}
