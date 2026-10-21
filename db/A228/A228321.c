/*
 * A228321  The Wiener index of the graph obtained by applying Mycielski's
 *          construction to the path graph on n vertices (n>=2).
 * https://oeis.org/A228321
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^2 - 13*n + 18 for n >= 3, a(2) = 15
 * Range:     u8 n <= 7, u16 n <= 105, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) {
        return n == 2 ? (seq_t)15 : (seq_t)0;
    }
    seq_t nn = (seq_t)n;
    return (seq_t)6 * nn * nn - (seq_t)13 * nn + (seq_t)18;
}
