/*
 * A156283  Period 6: repeat [1, 2, 4, -4, -2, -1].
 * https://oeis.org/A156283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [1, 2, 4, -4, -2, -1][n mod 6]
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t v[6] = {1, 2, 4, -4, -2, -1};
    return v[n % 6];
}
