/*
 * A089917  a(n) = 6^n *n! *L_n^{-1/6}(-1), where L_n^(alpha)(x) are
 *          generalized Laguerre polynomials.
 * https://oeis.org/A089917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (12*n - 1)*a(n-1) - (n-1)*(36*n - 42)*a(n-2) with a(0) =
 *            1, a(1) = 11 (OEIS A089917)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first two terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
 
    seq_t a0 = (seq_t)1;     /* a(n-2) */
    seq_t a1 = (seq_t)11;    /* a(n-1) */
    seq_t cur;
 
    for (idx_t i = 2; i <= n; ++i) {
        seq_t si  = (seq_t)i;          /* cast index to seq_t */
        seq_t im1 = si - (seq_t)1;     /* (n-1) as seq_t */
 
        /* term1 = (12*i - 1) * a1 */
        seq_t term1 = ((si * (seq_t)12) - (seq_t)1) * a1;
 
        /* term2 = (i-1) * (36*i - 42) * a0 */
        seq_t term2 = im1 * ((si * (seq_t)36) - (seq_t)42) * a0;
 
        cur = term1 - term2;
 
        a0 = a1;
        a1 = cur;
    }
    return a1;
}
