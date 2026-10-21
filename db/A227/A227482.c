/*
 * A227482  The Wiener index of the nanostar dendrimer defined pictorially as
 *          NS[n] in the M. Mirzargar reference.
 * https://oeis.org/A227482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -9369 + 2^n*74088 + 4^n*(50625*n-60075)
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 21, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n and 4^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) pow2 <<= 1;

    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i) pow4 <<= 2;

    /* term1 = 2^n * 74088 */
    seq_t term1 = pow2 * (seq_t)74088;

    /* term2 = 4^n * 50625 * n */
    seq_t term2 = pow4 * (seq_t)50625;
    term2 *= (seq_t)n;

    /* term3 = 4^n * 60075 */
    seq_t term3 = pow4 * (seq_t)60075;

    seq_t result = term1 + term2;
    result -= term3;
    result -= (seq_t)9369;

    return result;
}
