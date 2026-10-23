/*
 * A139594  Number of different n X n symmetric matrices with nonnegative
 *          entries summing to 4. Also number of symmetric oriented graphs
 *          with 4 arcs on n points.
 * https://oeis.org/A139594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2*(7 + 5*n^2)/12
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 194, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n * (seq_t)n;          /* n squared */
    seq_t term = 7 + (seq_t)5 * nn;           /* 7 + 5*n^2 */
    seq_t num = nn * term;                   /* n^2 * (7 + 5*n^2) */
    return num / 12;                         /* division is exact */
}
