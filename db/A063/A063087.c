/*
 * A063087  Dimension of the space of weight 2n cusp forms for Gamma_0( 19 ).
 * https://oeis.org/A063087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 1, a(n) = 4*(n-1) - 2*floor((n-1)/3) for n > 1
 *            (OEIS A063087)
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;               /* a(1) = 1 */

    seq_t nm1        = (seq_t)(n - 1);   /* n-1 (convert index to seq_t) */
    seq_t floordiv3  = nm1 / 3;          /* floor((n-1)/3) */
    return 4 * nm1 - 2 * floordiv3;     /* a(n) */
}
