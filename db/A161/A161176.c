/*
 * A161176  a(n) = 4n^2 - 10n + 107.
 * https://oeis.org/A161176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*n - 10*n + 107
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term1 = (seq_t)4 * nn * nn;
    seq_t term2 = (seq_t)10 * nn;
    return term1 + (seq_t)107 - term2;
}
