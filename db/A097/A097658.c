/*
 * A097658  Duplicate of A067725.
 * https://oeis.org/A097658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 3*n*(n+2)
 * Range:     i8 n <= 5, i16 n <= 18, i32 n <= 18, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x * (x + 2) * 3;
}
