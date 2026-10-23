/*
 * A162673  Number of different fixed (possibly) disconnected trominoes
 *          bounded (not necessarily tightly) by an n*n square.
 * https://oeis.org/A162673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(3*n^2-3*n-2)/2
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the arithmetic type */
    seq_t nn = (seq_t)n;
    /* Compute the parts of the expression */
    seq_t term1 = nn * (nn - (seq_t)1);
    seq_t term2 = (seq_t)3 * nn * nn - (seq_t)3 * nn - (seq_t)2;
    /* Combine and divide by 2 */
    seq_t result = term1 * term2 / (seq_t)2;
    return result;
}
