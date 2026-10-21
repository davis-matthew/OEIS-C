/*
 * A081273  Duplicate of A060544.
 * https://oeis.org/A081273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1 + 9 * n * (n-1) / 2
 * Range:     i8 n <= 5, i16 n <= 18, i32 n <= 18, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tmp = (seq_t)n * ((seq_t)n - 1) / 2;
    tmp = tmp * 9 + 1;
    return tmp;
}
