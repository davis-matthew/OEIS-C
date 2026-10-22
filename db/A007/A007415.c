/*
 * A007415  Expand sin x / exp x = x-x^2+x^3/3-x^5/30+... and invert nonzero
 *          coefficients.
 * https://oeis.org/A007415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [n mod 4 > 0] * (-1)^(n+1+⌊n/4⌋) * n! / 2^⌊n/2⌋
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if ((n & 3) == 0) {
        return 0;
    }
    
    idx_t exponent = n + 1 + (n >> 2);
    seq_t sign = (exponent & 1) ? -1 : 1;
    
    seq_t odd_product = 1;
    for (idx_t i = 1; i <= n; i += 2) {
        odd_product *= (seq_t)i;
    }
    
    seq_t factorial = 1;
    idx_t half_n = n >> 1;
    for (idx_t i = 1; i <= half_n; i++) {
        factorial *= (seq_t)i;
    }
    
    return sign * (odd_product * factorial);
}
