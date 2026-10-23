/*
 * A011799  Duplicate of A006124.
 * https://oeis.org/A011799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 3 + n*(7*n + 1)/2
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 11, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 3 + (nn * (7 * nn + 1)) / 2;
}
