/*
 * A117444  Period 5: Repeat [0, 1, 2, -2, -1].
 * https://oeis.org/A117444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = ((n+2) mod 5) - 2
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n % 5;
    m = (m + 2) % 5;
    return (seq_t)m - (seq_t)2;
}
