/*
 * A189890  a(n) = (n^3 - 2*n^2 + 3*n + 2)/2.
 * https://oeis.org/A189890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - 2*n^2 + 3*n + 2)/2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t */
    seq_t nn = (seq_t)n;

    /* compute numerator: n^3 - 2*n^2 + 3*n + 2 */
    seq_t term1 = nn * nn * nn;          /* n^3 */
    seq_t term2 = 2 * nn * nn;           /* 2*n^2 */
    seq_t term3 = 3 * nn;                /* 3*n */

    seq_t numerator = term1 - term2 + term3 + 2;
    return numerator / 2;
}
