/*
 * A214395  Decimal expansion of 16/27.
 * https://oeis.org/A214395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = {5, 9, 2}[n mod 3] (decimal digits of 16/27)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* cycle 5,9,2 repeats every 3 digits */
    static const seq_t digits[3] = {5, 9, 2};
    return digits[n % 3];
}
