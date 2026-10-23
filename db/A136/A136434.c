/*
 * A136434  Duplicate of A121670.
 * https://oeis.org/A136434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n^3 - 3*n
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 13, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return N * N * N - N * 3;
}
