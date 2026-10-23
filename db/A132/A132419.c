/*
 * A132419  Period 6: repeat [1, 1, -2, -1, -1, 2].
 * https://oeis.org/A132419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = {1, 1, -2, -1, -1, 2}[n mod 6]
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* periodic pattern of period 6 */
    static const seq_t tbl[6] = {1, 1, -2, -1, -1, 2};
    return tbl[n % 6];
}
