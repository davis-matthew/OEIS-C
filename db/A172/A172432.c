/*
 * A172432  Duplicate of A098952.
 * https://oeis.org/A172432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n for n <= 10, else a(n) = 2*n - 10
 * Range:     i8 n <= 18, i16 n <= 18, i32 n <= 18, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    return m <= 10 ? m : (2*m - 10);
}
