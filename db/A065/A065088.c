/*
 * A065088  a(n) = A000166(n)*binomial(n,2).
 * https://oeis.org/A065088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = subfactorial(n) * n*(n-1)/2 (A065088)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* binomial(n,2) is zero for n < 2, so the result is zero for n=0,1 */
    if (n < 2)
        return (seq_t)0;

    /* compute the derangement !n iteratively */
    seq_t d0 = (seq_t)1;   /* !0 */
    seq_t d1 = (seq_t)0;   /* !1 */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t di = (seq_t)(i - 1) * (d1 + d0); /* !i = (i-1)*(!(i-1)+!(i-2)) */
        d0 = d1;
        d1 = di;
    }
    /* d1 now holds !n */

    /* compute binomial(n,2) = n*(n-1)/2 */
    seq_t bin = (seq_t)n * ((seq_t)n - (seq_t)1) / (seq_t)2;

    return d1 * bin;
}
