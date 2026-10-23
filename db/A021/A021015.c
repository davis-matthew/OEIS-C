/*
 * A021015  Duplicate of A010680.
 * https://oeis.org/A021015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 0 for even n, 9 for odd n (i.e., a(n) = (n mod 2) * 9)
 * Range:     i8 n <= 26, i16 n <= 26, i32 n <= 26, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)((n & 1) ? 9 : 0);
}
