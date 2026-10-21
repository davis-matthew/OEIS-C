/*
 * A201236  Number of ways to place 2 non-attacking wazirs on an n X n
 *          toroidal board.
 * https://oeis.org/A201236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*(n^2-5)/2 for n>=3 a(1) = 0, a(2) = 2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 2;
    seq_t nn = (seq_t)n * (seq_t)n;           /* n^2 */
    seq_t temp = nn * (nn - (seq_t)5);        /* n^2 * (n^2-5) */
    return temp / (seq_t)2;                   /* divide by 2 */
}
