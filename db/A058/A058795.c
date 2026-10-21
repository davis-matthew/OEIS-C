/*
 * A058795  Row 4 of A007754.
 * https://oeis.org/A058795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 10*n^3 + 32*n^2 + 35*n + 7
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 253, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* convert index to seq_t */
    seq_t n2 = nn * nn;            /* n^2 */
    seq_t n3 = n2 * nn;            /* n^3 */
    seq_t n4 = n3 * nn;            /* n^4 */
    return n4
           + ((seq_t)10 * n3)
           + ((seq_t)32 * n2)
           + ((seq_t)35 * nn)
           + (seq_t)7;
}
