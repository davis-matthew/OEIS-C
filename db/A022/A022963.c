/*
 * A022963  a(n) = 7-n.
 * https://oeis.org/A022963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 7 - n
 * Range:     i8 n <= 59, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)7 - (seq_t)n;
}
