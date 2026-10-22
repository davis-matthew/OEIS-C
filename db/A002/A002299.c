/*
 * A002299  Binomial coefficients C(2*n+5,5).
 * https://oeis.org/A002299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2*n+5, 5)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 107, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = 2 * (seq_t)n + 5;
    seq_t result = 1;
    
    for (seq_t k = 1; k <= 5; k++) {
        result = result * (m - k + 1) / k;
    }
    
    return result;
}
