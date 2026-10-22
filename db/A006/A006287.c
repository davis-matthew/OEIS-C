/*
 * A006287  Sum of squares of digits of ternary representation of n.
 * https://oeis.org/A006287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of squares of digits in base-3 representation of n
 * Range:     u8 n <= 500, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t temp = n;
    while (temp > 0) {
        seq_t digit = (seq_t)(temp % 3);
        result += digit * digit;
        temp /= 3;
    }
    return result;
}
