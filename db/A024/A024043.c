/*
 * A024043  a(n) = 4^n - n^7.
 * https://oeis.org/A024043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^n - n^7
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 4^n */
    seq_t pow4 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow4 *= (seq_t)4;

    /* compute n^7 */
    seq_t pow7 = (seq_t)n;
    for (int i = 1; i < 7; ++i)
        pow7 *= (seq_t)n;

    return pow4 - pow7;
}
