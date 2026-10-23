/*
 * A128091  Row sums of unsigned A128090.
 * https://oeis.org/A128091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^2 - 2*n + 2)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned; cast to seq_t before arithmetic to avoid promotion to a
     * smaller type.  The formula works for n >= 1; for n == 0 return 0
     * (offset is 1, so values below offset are irrelevant). */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;               /* n^2 */
    seq_t inner = sq - (seq_t)2 * nn + (seq_t)2;   /* n^2 - 2n + 2 */
    return sq * inner;                /* n^2 * (n^2 - 2n + 2) */
}
