/*
 * A173704  Partial sums of floor(n^3/2).
 * https://oeis.org/A173704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n^4+2*n^3+n^2-2*n)/8)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 429, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* n */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n4 = n3 * nn;           /* n^4 */
    return (n4 + 2 * n3 + n2 - 2 * nn) / 8;
}
