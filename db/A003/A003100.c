/*
 * A003100  Decimal Gray code for n.
 * https://oeis.org/A003100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   Decimal Gray code - a(n) reflects digit based on whether the
 *            higher-order part is odd
 * Range:     u8 n <= 249, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 10) {
        return (seq_t)n;
    }
    
    idx_t higher = n / 10;
    idx_t last_digit = n % 10;
    
    seq_t higher_result = a(higher);
    
    if (higher % 2 == 1) {
        return higher_result * 10 + (seq_t)(9 - last_digit);
    } else {
        return higher_result * 10 + (seq_t)last_digit;
    }
}
