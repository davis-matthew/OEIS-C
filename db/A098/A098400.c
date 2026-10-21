/*
 * A098400  a(n) = 4^n*binomial(2*n+1, n).
 * https://oeis.org/A098400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n * binomial(2*n+1, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 4^n */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow4 *= 4;

    /* compute binomial(2*n+1, n) multiplicatively */
    seq_t bin = 1;
    for (idx_t k = 1; k <= n; ++k)
        bin = bin * (n + 1 + k) / k;

    /* combine the results */
    return pow4 * bin;
}
