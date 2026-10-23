/*
 * A157798  a(n) = 1482401250*n^2 - 658736100*n + 73180801.
 * https://oeis.org/A157798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1482401250*n^2 - 658736100*n + 73180801
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute nn * nn first to avoid overflow of the constant product */
    seq_t n2 = nn * nn;
    seq_t term1 = (seq_t)1482401250 * n2;
    seq_t term2 = (seq_t)658736100 * nn;
    return term1 - term2 + (seq_t)73180801;
}
