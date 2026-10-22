/*
 * A006352  Coefficients in expansion of Eisenstein series E_2 (also called
 *          E_1 or G_2).
 * https://oeis.org/A006352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = 1 if n = 0, else -24*sigma(n) where sigma(n) is sum of
 *            divisors
 * Range:     i8 n <= 3, i16 n <= 479, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    
    // Compute sigma(n) = sum of divisors of n
    seq_t sigma = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            sigma += (seq_t)i;
            if (i * i != n) {
                sigma += (seq_t)(n / i);
            }
        }
    }
    
    return -24 * sigma;
}
