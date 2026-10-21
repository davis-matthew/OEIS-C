/*
 * A063130  Dimension of the space of weight 2n cusp forms for Gamma_0( 62 ).
 * https://oeis.org/A063130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 7, a(n) = 2*(8*n-5) for n > 1 (i.e. a(n) = 16*n-10)
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute term a(n) of OEIS A063130 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* not defined by OEIS, safe fallback */
    if (n == 1) return (seq_t)7;           /* a(1) = 7 */
    /* For n > 1 use the explicit formula: a(n) = 16*n - 10 */
    return (seq_t)16 * (seq_t)n - (seq_t)10;
}
