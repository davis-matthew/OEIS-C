/*
 * A059673  Sum of binary numbers with n 1's and one (non-leading) 0.
 * https://oeis.org/A059673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = (2n-1)*2^n + 1 - n for n>0; a(0)=0
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* compute 2^n as a seq_t */
    seq_t two_n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        two_n <<= 1;          /* shift left by 1, i.e. multiply by 2 */

    /* a(n) = ((2n-1)*2^n) + 1 - n   (all intermediate values in seq_t) */
    seq_t term = ((seq_t)(2 * n - 1) * two_n) + (seq_t)1 - (seq_t)n;
    return term;
}
