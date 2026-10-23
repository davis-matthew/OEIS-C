/*
 * A192342  Constant term of the reduction of n-th polynomial at A158983 by
 *          x^2->x+2.
 * https://oeis.org/A192342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = constant term of the reduction of p_{n-1}(x) * where
 *            p_0(x)=2+x and p_{k+1}(x)=p_k(x)^2+1, * reduction performed
 *            modulo x^2 - x - 2. * Recurrence for the reduced coefficients: *
 *            c0_{k+1} = c0_k^2 + 2*c1_k^2 + 1 * c1_{k+1} = 2*c0_k*c1_k +
 *            c1_k^2 * with c0_0 = 2, c1_0 = 1. Then a(n) = c0_{n-1}
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n - 1;          /* n>=1, so k is safe */
    seq_t c0 = 2;
    seq_t c1 = 1;

    for (idx_t i = 0; i < k; ++i) {
        seq_t c1sq = c1 * c1;
        seq_t c0sq = c0 * c0;

        seq_t new_c0 = c0sq + c1sq + c1sq + 1;          /* c0^2 + 2*c1^2 + 1 */
        seq_t new_c1 = (c0 * c1) + (c0 * c1) + c1sq;    /* 2*c0*c1 + c1^2 */

        c0 = new_c0;
        c1 = new_c1;
    }
    return c0;
}
