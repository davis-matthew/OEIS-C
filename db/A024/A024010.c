/*
 * A024010  a(n) = 1 - n^12.
 * https://oeis.org/A024010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - n^12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 38, i128 n <= 320,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;                /* start with n */
    for (idx_t i = 1; i < 12; ++i)     /* multiply n eleven more times → n^12 */
        x *= (seq_t)n;
    return (seq_t)1 - x;                /* 1 - n^12 */
}
