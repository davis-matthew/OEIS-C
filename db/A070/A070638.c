/*
 * A070638  Duplicate of A070438.
 * https://oeis.org/A070638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n^2 mod 15
 * Range:     i8 n <= 96, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 15 to avoid overflow */
    idx_t r = n % 15;
    /* Compute r^2 modulo 15 */
    idx_t sq = (r * r) % 15;
    return (seq_t)sq;
}
