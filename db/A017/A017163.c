/*
 * A017163  a(n) = (9*n)^3.
 * https://oeis.org/A017163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n)^3
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 180, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* convert idx_t to seq_t safely */
    seq_t n2 = nn * nn;             /* n^2 */
    seq_t n3 = n2 * nn;             /* n^3 */
    return ((seq_t)729) * n3;       /* 729 * n^3 = (9*n)^3 */
}
