/*
 * A081590  Fourth row of Pascal-(1,5,1) array A081580.
 * https://oeis.org/A081590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 36*n - 54*n^2 + 36*n^3
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 492, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t nn = (seq_t)n;

    /* Compute 36*n^3 + 36*n + 1  (always ≥ 54*n^2 for n ≥ 0) */
    seq_t term1 = 36 * nn * nn * nn;
    term1 += 36 * nn;
    term1 += 1;

    /* Subtract 54*n^2 */
    seq_t term2 = 54 * nn * nn;
    return term1 - term2;
}
