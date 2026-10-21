/*
 * A082113  a(n) = n^4 + 5*n^2 + 1.
 * https://oeis.org/A082113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^4 + 5*n^2 + 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;      /* n^2 */
    seq_t nn4 = nn2 * nn2;    /* n^4 */
    seq_t term = nn4 + (seq_t)5 * nn2 + (seq_t)1;
    return term;
}
