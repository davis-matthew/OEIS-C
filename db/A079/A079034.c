/*
 * A079034  Determinant of M(n), the n X n matrix defined by m(i,i) = 1,
 *          m(i,j) = i-j.
 * https://oeis.org/A079034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - n^2 + 12) / 12
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;          /* convert index to seq_t */
    seq_t n2  = nn * nn;           /* n^2 */
    seq_t n4  = n2 * n2;           /* n^4 */
    seq_t val = n4 - n2 + (seq_t)12; /* n^4 - n^2 + 12 */
    return val / (seq_t)12;        /* integer division */
}
