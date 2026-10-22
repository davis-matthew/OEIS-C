/*
 * A007091  Numbers in base 5.
 * https://oeis.org/A007091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   Convert n to base 5, then interpret base-5 digits as decimal
 *            digits
 * Range:     u8 n <= 74, u16 n <= 3124, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power = 1;
    while (n > 0) {
        seq_t digit = (seq_t)(n % 5);
        result += digit * power;
        power *= 10;
        n /= 5;
    }
    return result;
}
