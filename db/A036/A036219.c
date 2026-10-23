/*
 * A036219  Expansion of 1/(1-3*x)^6; 6-fold convolution of A000244 (powers of
 *          3).
 * https://oeis.org/A036219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n * binomial(n+5, 5)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 28, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute binomial(n+5, 5) iteratively */
    seq_t binom = 1;
    idx_t N = n + 5;
    for (idx_t k = 1; k <= 5; ++k)
        binom = binom * (seq_t)(N - k + 1) / (seq_t)k;

    /* compute 3^n */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= 3;

    return pow3 * binom;
}
