/*
 * A131649  Number of distinct improper 2-coloring of edges for odd-order
 *          cyclic graphs.
 * https://oeis.org/A131649
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n^2 - 17*n + 26 (conjectured for n>4)
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)8;
    seq_t nn = (seq_t)n;
    return (seq_t)3 * nn * nn - (seq_t)17 * nn + (seq_t)26;
}
