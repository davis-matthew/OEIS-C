/*
 * A024001  a(n) = 1 - n^3.
 * https://oeis.org/A024001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - n^3
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 730, i64 n <= 730,
 *            i128 n <= 730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ni = (seq_t)n;
    return (seq_t)1 - (ni * ni * ni);
}
