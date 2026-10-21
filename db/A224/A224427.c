/*
 * A224427  The Wiener index of the dendrimer NS[n], defined pictorially in
 *          the A. R. Ashrafi et al. reference.
 * https://oeis.org/A224427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -1113 + 8112*2^n - 6656*4^n + 5120*n*4^n
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= (seq_t)2;

    /* compute 4^n = (2^n)^2 */
    seq_t pow4 = pow2;
    pow4 *= pow2;

    /* terms of the formula */
    seq_t term1 = pow2 * (seq_t)8112;          /* 8112 * 2^n */
    seq_t term2 = pow4 * (seq_t)5120;          /* 5120 * 4^n */
    term2 *= (seq_t)n;                         /* * n */
    seq_t term3 = pow4 * (seq_t)6656;          /* 6656 * 4^n */

    /* combine: -1113 + term1 + term2 - term3 */
    seq_t result = term1 + term2;
    result -= term3;
    result -= (seq_t)1113;

    return result;
}
