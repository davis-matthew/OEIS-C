/*
 * A093620  Values of Laguerre polynomials: a(n) =
 *          2^n*n!*LaguerreL(n,-1/2,-2).
 * https://oeis.org/A093620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 5 a(n) = (4*n + 1) * a(n-1) - 2*(n-1)*(2*n - 3)
 *            * a(n-2) for n >= 2 This follows from a(n) = 2^n n!
 *            LaguerreL(n,-1/2,-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a_prev2 = (seq_t)1;   // a(0)
    seq_t a_prev1 = (seq_t)5;   // a(1)
    seq_t a_cur   = (seq_t)0;

    seq_t two   = (seq_t)2;
    seq_t three = (seq_t)3;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ci = (seq_t)i;

        /* coeff1 = 4*i + 1 */
        seq_t coeff1 = (seq_t)4 * ci + (seq_t)1;

        /* coeff2 = 2*(i-1)*(2*i - 3) */
        seq_t coeff2 = two * (ci - (seq_t)1) * ((seq_t)2 * ci - three);

        /* a_cur = coeff1 * a_prev1 - coeff2 * a_prev2 */
        seq_t term1 = coeff1 * a_prev1;
        seq_t term2 = coeff2 * a_prev2;
        a_cur = term1 - term2;

        /* shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_cur;
}
