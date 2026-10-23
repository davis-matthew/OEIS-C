/*
 * A163832  a(n) = n*(2*n^2 + 5*n + 1).
 * https://oeis.org/A163832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A163832
   Formula: a(n) = n*(2*n^2 + 5*n + 1)
*/
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* compute inner part: 2*n^2 + 5*n + 1 */
    seq_t inner = (seq_t)2 * nn * nn + (seq_t)5 * nn + (seq_t)1;
    return nn * inner;
}
