/*
 * A001850  Central Delannoy numbers: a(n) = Sum_{k=0..n} C(n,k)*C(n+k,k).
 * https://oeis.org/A001850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{i=0..n} (2^i * C(n, i)^2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t binom = 1;
    seq_t pow2 = 1;
    
    for (idx_t i = 0; i <= n; i++) {
        seq_t term = binom * binom * pow2;
        result = result + term;
        
        if (i < n) {
            binom = binom * (seq_t)(n - i) / (seq_t)(i + 1);
            pow2 = pow2 * 2;
        }
    }
    return result;
}
