/*
 * A003132  Sum of squares of digits of n.
 * https://oeis.org/A003132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look, base, nice
 * Formula:   a(n) = sum of squares of digits of n
 * Range:     u8 n <= 1045, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t num = (seq_t)n;
    
    while (num > 0) {
        seq_t digit = num % 10;
        result += digit * digit;
        num /= 10;
    }
    
    return result;
}
