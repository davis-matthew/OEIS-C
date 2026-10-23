/*
 * A159075  a(1) = -1, otherwise a(n) = 0.
 * https://oeis.org/A159075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 0, a(1) = -1, a(n) = 0 for n >= 2
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 1) ? (seq_t)-1 : (seq_t)0;
}
