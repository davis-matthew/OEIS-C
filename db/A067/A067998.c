/*
 * A067998  a(n) = n^2 - 2*n.
 * https://oeis.org/A067998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n*(n-2) (offset 0)
 * Range:     i8 n <= 12, i16 n <= 182, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t for arithmetic */
    return nn * (nn - 2);
}
