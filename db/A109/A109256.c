/*
 * A109256  a(n) = n^6 - 11n^4 + 36n^2 - 36.
 * https://oeis.org/A109256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n^2 - 2) * (n^2 - 3) * (n^2 - 6)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 35, i64 n <= 1448,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    return (n2 - (seq_t)2) * (n2 - (seq_t)3) * (n2 - (seq_t)6);
}
