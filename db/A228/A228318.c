/*
 * A228318  The Wiener index of the graph obtained by applying Mycielski's
 *          construction to the star graph K(1,n).
 * https://oeis.org/A228318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 + 6*n + 5
 * Range:     u8 n <= 7, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 4 * nn * nn + 6 * nn + 5;
}
