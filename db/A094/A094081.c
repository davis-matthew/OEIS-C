/*
 * A094081  Smallest integral ladder whose ends slide over the respective
 *          distances 1 and m=2*n+1 while slipping down along horizontal
 *          ground and vertical wall against which it leans.
 * https://oeis.org/A094081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 + 2*n + 1) * (16*n^2 + 16*n + 5)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* cast index to seq_t first */
    seq_t term1 = 2 * nn * nn + 2 * nn + 1;
    seq_t term2 = 16 * nn * nn + 16 * nn + 5;
    return term1 * term2;
}
