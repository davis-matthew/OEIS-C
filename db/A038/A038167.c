/*
 * A038167  G.f.: x*(1+3*x+x^2)/((1-x^2)^2*(1-x^5)).
 * https://oeis.org/A038167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{m=0}^{⌊n/5⌋} Σ_{d=1}^3 * c_d * ( (n - 5m - d)/2 + 1 )
 *            * if n-5m-d ≥ 0 and (n-5m-d) is even; * otherwise 0. *
 *            Coefficients: c_1 = 1, c_2 = 3, c_3 = 1. * This implements the
 *            generating function * A038167 :
 *            x*(1+3*x+x^2)/((1-x^2)^2*(1-x^5))
 * Range:     u8 n <= 43, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* Loop over the 5*m part of the exponent */
    for (idx_t m = 0; m <= n / 5; ++m) {
        idx_t t = n - 5 * m;          /* remaining exponent after 5*m */
        /* Check d = 1,2,3 from the numerator */
        for (idx_t d = 1; d <= 3; ++d) {
            if (t < d)                 /* not enough power of x left */
                continue;
            idx_t diff = t - d;
            if (diff & 1)              /* must be even to be 2k */
                continue;
            idx_t k = diff >> 1;       /* k = (n - 5m - d) / 2 */
            seq_t coeff = (d == 2 ? (seq_t)3 : (seq_t)1);
            result += coeff * ((seq_t)k + 1);
        }
    }
    return result;
}
