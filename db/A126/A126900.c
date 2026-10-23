/*
 * A126900  Coordination sequence for 6-dimensional cyclotomic lattice
 *          Z[zeta_18].
 * https://oeis.org/A126900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{6} coeff[k] * binom(n-k+5,5) with coeff =
 *            {1,12,51,88,51,12,1} and binom(m,5) = m(m-1)(m-2)(m-3)(m-4)/120
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Coefficients of (x^2+4x+1)^3 */
    const int coeff[7] = {1, 12, 51, 88, 51, 12, 1};

    seq_t sum = 0;

    /* Compute the sum for k = 0..min(6,n) */
    for (idx_t k = 0; k <= 6 && k <= n; ++k)
    {
        idx_t m = n - k + 5;          /* m >= 5 here */

        /* binom(m,5) computed multiplicatively */
        seq_t r = 1;
        r *= (seq_t)(m);
        r *= (seq_t)(m - 1);
        r *= (seq_t)(m - 2);
        r *= (seq_t)(m - 3);
        r *= (seq_t)(m - 4);
        r /= (seq_t)120;             /* exact division by 5! */

        sum += (seq_t)coeff[k] * r;
    }

    return sum;
}
