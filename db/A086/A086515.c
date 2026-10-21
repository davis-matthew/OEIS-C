/*
 * A086515  Duplicate of A047241.
 * https://oeis.org/A086515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 3*n - 3 + (n mod 2) (for n >= 1)
 * Range:     i8 n <= 43, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * 3 - 3 + (n & 1);
}
