/*
 * A238469  Period 7: repeat [0, 1, 0, 0, 0, 0, -1].
 * https://oeis.org/A238469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [0, 1, 0, 0, 0, 0, -1][n mod 7]
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[7] = {0, 1, 0, 0, 0, 0, (seq_t)-1};
    return period[n % 7];
}
