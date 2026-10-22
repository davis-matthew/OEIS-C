/*
 * A001902  Successive denominators of Wallis's approximation to Pi/2
 *          (reduced).
 * https://oeis.org/A001902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator(W(n)), where W(n) = Product_{k=0..n}
 *            N(k)/D(k) (reduced), with N(k) = 2*floor((k+1)/2) for k >= 1 and
 *            N(0) = 1, and D(k) = 2*floor(k/2) + 1 for k >= 0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_seq(seq_t a, seq_t b) {
    while (b) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t numerator = 1, denominator = 1;
    
    for (idx_t k = 0; k <= n; k++) {
        seq_t numer_k, denom_k;
        
        if (k == 0) {
            numer_k = 1;
        } else {
            numer_k = 2 * ((k + 1) >> 1);
        }
        denom_k = 2 * (k >> 1) + 1;
        
        // Cross-reduction to avoid overflow
        seq_t g1 = gcd_seq(numerator, denom_k);
        numerator /= g1;
        denom_k /= g1;
        
        seq_t g2 = gcd_seq(numer_k, denominator);
        numer_k /= g2;
        denominator /= g2;
        
        // Multiply
        numerator *= numer_k;
        denominator *= denom_k;
    }
    
    return denominator;
}
