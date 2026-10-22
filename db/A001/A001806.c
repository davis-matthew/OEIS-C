/*
 * A001806  a(n) = n! * binomial(n,4).
 * https://oeis.org/A001806
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * binomial(n, 4)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute binomial(n, 4) = n(n-1)(n-2)(n-3) / 24
    seq_t binom = (seq_t)n;
    binom *= (seq_t)(n - 1);
    binom /= 2;
    binom *= (seq_t)(n - 2);
    binom /= 3;
    binom *= (seq_t)(n - 3);
    binom /= 4;
    
    // Compute n!
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; i++) {
        fact *= (seq_t)i;
    }
    
    return fact * binom;
}
