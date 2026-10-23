/*
 * A121672  Real part of (n + i)^5.
 * https://oeis.org/A121672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(n^4 - 10*n^2 + 5)
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 73, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* cast index to target type */
    seq_t n2 = sn * sn;           /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */
    seq_t val = n4 - (seq_t)10 * n2 + (seq_t)5;
    return sn * val;
}
