/*
 * A082946  Duplicate of A082945.
 * https://oeis.org/A082946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 101*n + 10
 * Range:     i8 n <= 1, i16 n <= 9, i32 n <= 9, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)101 * (seq_t)n) + (seq_t)10;
}
