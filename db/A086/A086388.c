/*
 * A086388  Duplicate of A008619.
 * https://oeis.org/A086388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (n+1)/2
 * Range:     i8 n <= 29, i16 n <= 29, i32 n <= 29, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n + 1) / 2;
}
