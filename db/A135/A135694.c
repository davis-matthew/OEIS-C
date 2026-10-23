/*
 * A135694  Period 6: repeat [1, -1, -1, -1, 0, 2].
 * https://oeis.org/A135694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   period 6: repeat [1, -1, -1, -1, 0, 2]
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t table[6] = {1, -1, -1, -1, 0, 2};
    return table[n % 6];
}
