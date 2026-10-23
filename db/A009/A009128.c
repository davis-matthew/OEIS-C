/*
 * A009128  Expansion of e.g.f. cosh(log(1+x))*cos(x).
 * https://oeis.org/A009128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes coefficients a(n) of the exponential generating function
    cosh(log(1+x))*cos(x).  The function uses only integer arithmetic.
    For each index n it evaluates
        a(n) = sum_{j=0..⌊n/2⌋} h_{n-2j} * (-1)^j * n! / (2j)!
    where h_m = 1          (m==0)
                   0          (m==1)
                   1/2        (m>=2 and m even)
                   -1/2       (m>=3 and m odd).
    The result is returned as type seq_t (signed).
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0) return 1;
    if (n == 1) return 0;

    /* Compute n! as seq_t */
    seq_t fac_n = 1;
    for (idx_t i = 2; i <= n; ++i)
        fac_n *= (seq_t)i;

    seq_t res   = 0;
    seq_t ratio = fac_n;            /* ratio = fac_n / fac_{2j}  */
    idx_t J     = n / 2;            /* upper bound on j */

    for (idx_t j = 0; j <= J; ++j) {
        idx_t m = n - 2 * j;        /* m = n-2j */

        if (m != 1) {              /* h_m = 0 only when m==1   */
            /* sign from (-1)^j  */
            int overall_sign = (j & 1) ? -1 : 1;

            /* sign from h_m  */
            int h_sign;
            if (m == 0)
                h_sign = 1;          /* h_0 = 1          */
            else if ((m & 1) == 0)  /* m even >=2        */
                h_sign = 1;          /* h_m = 1/2        */
            else                    /* m odd >=3         */
                h_sign = -1;         /* h_m = -1/2       */

            int sign_term = overall_sign * h_sign;

            /* term = ratio * sign_term  then divide by 2 if m>=2 */
            seq_t term = ratio * (seq_t)sign_term;
            if (m >= 2)
                term = term / 2;

            res += term;
        }

        /* Update ratio for next j: divide by (2j+1)*(2j+2) */
        if (j < J) {
            idx_t a1 = 2 * j + 1;
            idx_t a2 = 2 * j + 2;
            seq_t mult = (seq_t)a1 * (seq_t)a2;
            ratio = ratio / mult;
        }
    }

    return res;
}
