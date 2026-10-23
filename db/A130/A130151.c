/*
 * A130151  Period 6: repeat [1, 1, 1, -1, -1, -1].
 * https://oeis.org/A130151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^floor(n/3)
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n / 3;
    if (t & 1)
        return (seq_t)-1;
    else
        return (seq_t)1;
}
