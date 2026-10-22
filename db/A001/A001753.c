/*
 * A001753  Expansion of 1/((1+x)*(1-x)^6).
 * https://oeis.org/A001753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum{k=0..n} (-1)^(n-k)*binomial(k+5, 5)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 249, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t k = 0; k <= n; k++) {
        // Compute binomial(k+5, 5) = (k+1)*(k+2)*(k+3)*(k+4)*(k+5) / 120
        // by multiplying and dividing iteratively to avoid overflow
        seq_t term = (seq_t)(k + 1);
        term *= (seq_t)(k + 2);
        term /= 2;
        term *= (seq_t)(k + 3);
        term /= 3;
        term *= (seq_t)(k + 4);
        term /= 4;
        term *= (seq_t)(k + 5);
        term /= 5;
        
        // Add or subtract based on (-1)^(n-k)
        if ((n - k) & 1) {
            result -= term;
        } else {
            result += term;
        }
    }
    return result;
}
