/*
 * A007452  Expand cos x / exp x and invert nonzero coefficients.
 * https://oeis.org/A007452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^[n == 1, 4, 7 mod 8] * n!/2^floor(n/2); a(4n+2) = 0
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 14, i64 n <= 22, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n % 4 == 2) return 0;
    
    seq_t result = 1;
    idx_t power_of_2_remaining = n / 2;
    
    for (idx_t i = 1; i <= n; i++) {
        result *= (seq_t)i;
        
        if (power_of_2_remaining > 0 && result % 2 == 0) {
            result /= 2;
            power_of_2_remaining--;
        }
    }
    
    if (n % 8 == 1 || n % 8 == 4 || n % 8 == 7) {
        result = -result;
    }
    
    return result;
}
