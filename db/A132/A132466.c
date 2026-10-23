/*
 * A132466  Let df(n,k) = Product_{i=0..k-1} (n-i) be the descending factorial
 *          and let P(m,n) = df(n-1,m-1)^2*(2*n-m)/((m-1)!*m!). Sequence gives
 *          P(8,n).
 * https://oeis.org/A132466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n-1,7)^2 * (n-4)/4 (zero for n<8)
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 7) return 0;

    seq_t bin = 1;
    /* compute C(n-1, 7) iteratively */
    for (idx_t i = 1; i <= 7; ++i) {
        bin = (bin * (seq_t)(n - i)) / (seq_t)i;
    }

    seq_t val = bin * bin;            /* square the binomial */
    val = (val * (seq_t)(n - 4)) / (seq_t)4;  /* multiply by (n-4)/4 */
    return val;
}
