/*
 * A131554  Period 5: repeat [1, 1, -1, 1, -1].
 * https://oeis.org/A131554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = pattern[n mod 5], pattern = [1, 1, -1, 1, -1]
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic table of values */
    static const seq_t tbl[5] = { 1, 1, -1, 1, -1 };
    return tbl[n % 5];
}
