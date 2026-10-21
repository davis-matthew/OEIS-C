/*
 * A215544  Number of standard Young tableaux of shape [4n,4].
 * https://oeis.org/A215544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0, n = 0; a(n) = (4*n-3)*(4*n+3)*(2*n+1)*(n+1)/3 for n >
 *            0
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 140, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t nm = (seq_t)n;                /* convert index to seq_t */
    seq_t nplus1 = nm + 1;
    seq_t four_n = nm * 4;

    /* Compute the product (4n-3)*(4n+3)*(2n+1)*(n+1) */
    seq_t term = (four_n - 3) * (four_n + 3);
    term = term * (2 * nm + 1);
    term = term * nplus1;

    /* Divide by 3 to get the final value */
    term = term / 3;
    return term;
}
