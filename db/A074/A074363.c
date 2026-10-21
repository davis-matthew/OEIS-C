/*
 * A074363  Coefficient of q^3 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(3,1).
 * https://oeis.org/A074363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of q^3 in nu(n) where nu(0)=1, nu(1)=3, and
 *            for n>=2: nu(n) = 3 * nu(n-1) + (1 + q + q^2 + ... + q^(n-2)) *
 *            nu(n-2) (lambda=1) The algorithm keeps the four lowest
 *            coefficients of every nu(n) and updates them in O(n) time
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)0;        /* a(0)=0, a(1)=0 */

    /* coeffs for nu(n-2), nu(n-1), and nu(n) */
    seq_t prev2[4] = { (seq_t)1, 0, 0, 0 };  /* nu(0)   */
    seq_t prev1[4] = { (seq_t)3, 0, 0, 0 };  /* nu(1)   */
    seq_t curr[4]  = { 0, 0, 0, 0 };

    for (idx_t i = 2; i <= n; ++i) {
        /* compute coefficients for nu(i) */
        for (int k = 0; k < 4; ++k) {
            seq_t sum = 0;
            int limit = (k < (int)(i - 2)) ? k : (int)(i - 2);  /* min(k, n-2) */
            for (int t = 0; t <= limit; ++t)
                sum += prev2[k - t];
            curr[k] = prev1[k] * (seq_t)3 + sum;
        }
        /* shift arrays for next iteration */
        for (int k = 0; k < 4; ++k) {
            prev2[k] = prev1[k];
            prev1[k] = curr[k];
        }
    }

    return prev1[3];   /* coefficient of q^3 */
}
