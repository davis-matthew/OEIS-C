/*
 * A133513  Period 6: repeat [0, 1, -3, 0, -1, 3].
 * https://oeis.org/A133513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) repeats the pattern [0, 1, -3, 0, -1, 3] with period 6
 *            (offset 0)
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[6] = {0, 1, -3, 0, -1, 3};
    /* n is unsigned; modulo 6 yields 0..5 */
    return period[n % 6];
}
