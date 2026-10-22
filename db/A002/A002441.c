/*
 * A002441  Squares written in base 8.
 * https://oeis.org/A002441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A007094(A000290(n))
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t square = (seq_t)n * (seq_t)n;
    seq_t octal_digits[32];
    int num_digits = 0;
    seq_t temp = square;
    
    while (temp > 0) {
        octal_digits[num_digits++] = temp % 8;
        temp /= 8;
    }
    
    seq_t result = 0;
    for (int i = num_digits - 1; i >= 0; i--) {
        result = result * 10 + octal_digits[i];
    }
    
    return result;
}
