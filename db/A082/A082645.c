/*
 * A082645  a(n) = floor((2*n^2 + n - 4)/4).
 * https://oeis.org/A082645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor((2*n^2 + n - 4)/4)
 * Range:     i8 n <= 7, i16 n <= 161, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t num = 2*tn*tn + tn - 4;
    if (num >= 0)
        return num / 4;
    else
        return - ((-num + 3) / 4);
}
