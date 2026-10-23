/*
 * A133070  a(n) = n^5 - n^3 - n^2.
 * https://oeis.org/A133070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^5 - n^3 - n^2
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 73, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n5 = n3 * n2;   /* n^5 = n^3 * n^2 */
    return n5 - n3 - n2;
}
