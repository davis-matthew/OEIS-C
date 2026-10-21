/*
 * A083594  a(n) = (7 - 4*(-2)^n)/3.
 * https://oeis.org/A083594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (7 - 4*(-2)^n)/3
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 28, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t power = 1;                /* (-2)^0 = 1 */
    for (idx_t i = 0; i < n; ++i)
    {
        power *= (seq_t)-2;         /* multiply by -2 each step */
    }
    return ((seq_t)7 - (seq_t)4 * power) / (seq_t)3;
}
