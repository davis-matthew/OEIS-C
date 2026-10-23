/*
 * A022960  a(n) = 4-n.
 * https://oeis.org/A022960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4 - n
 * Range:     i8 n <= 58, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)4 - (seq_t)n;
}
