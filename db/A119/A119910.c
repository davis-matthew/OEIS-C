/*
 * A119910  Period 6: repeat [1, 3, 2, -1, -3, -2].
 * https://oeis.org/A119910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) repeats the period [1, 3, 2, -1, -3, -2] with period 6
 * Range:     i8 n <= 84, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[6] = { 1, 3, 2, -1, -3, -2 };
    return period[(n - 1) % 6];
}
