/*
 * A021037  Duplicate of A010674.
 * https://oeis.org/A021037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 3 * (n % 2)
 * Range:     i8 n <= 98, i16 n <= 98, i32 n <= 98, i64 n <= 98, i128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)((n & 1) * 3);
}
