/*
 * A054607  a(n) = Sum_{d|8} phi(d)*n^(8/d).
 * https://oeis.org/A054607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 + n^4 + 2*n^2 + 4*n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */
    seq_t n8 = n4 * n4;           /* n^8 */
    seq_t term = n8 + n4 + ((seq_t)2 * n2) + ((seq_t)4 * nn);
    return term;
}
