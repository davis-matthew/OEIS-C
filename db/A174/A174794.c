/*
 * A174794  a(0) = 0 and a(n) = (4*n^3 - 12*n^2 + 20*n - 9)/3 for n >= 1.
 * https://oeis.org/A174794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 - 12*n^2 + 20*n - 9)/3 for n >= 1, a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;

    /* compute numerator as 4*n^3 + 20*n - (12*n^2 + 9) to avoid
       negative intermediate values when seq_t is unsigned   */
    seq_t term1 = 4 * nn * nn * nn + 20 * nn;
    seq_t term2 = 12 * nn * nn + 9;
    seq_t num   = term1 - term2;

    return num / 3;
}
