/*
 * A009572  Expansion of e.g.f. sinh(log(1+x))*cos(x).
 * https://oeis.org/A009572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * Σ_{i=1}^{n} g_i * d_{n-i} where g_1 = 1, g_i =
 *            (-1)^{i-1}/2 for i ≥ 2 d_{2m} = (-1)^m/(2m)! and d_{odd} = 0.
 *            The implementation evaluates the sum directly in integer
 *            arithmetic and multiplies by n! to obtain the exact e.g.f
 *            coefficients
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 13, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) = 0 */
        return 0;

    seq_t sum = 0;

    /* factorial of n */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    /* contribution of i = 1 (g_1 = 1) */
    if (n & 1) { /* n is odd */
        idx_t exp = (n - 1) / 2;          /* exponent for sign */
        int sign = (exp & 1) ? -1 : 1;     /* (-1)^exp */
        sum += (seq_t)n * ((seq_t)sign);
    }

    /* contributions for i ≥ 2 */
    if (n >= 2) {
        /* maximum m such that i = n - 2m ≥ 2 */
        idx_t m_max = (n - 2) / 2;

        /* product = ∏_{k=2*m_max+1}^{n} k  = n! / (2*m_max)! */
        seq_t prod = 1;
        for (idx_t k = 2 * m_max + 1; k <= n; ++k)
            prod *= (seq_t)k;

        for (idx_t m = m_max; ; --m) {
            /* sign = (-1)^{n-1-m} */
            int sign = ((n - 1 - m) & 1) ? -1 : 1;

            /* term = prod / 2 with appropriate sign */
            seq_t term = prod / 2;
            if (sign == -1) term = -term;

            sum += term;

            if (m == 0) break;           /* finished all terms */

            /* update prod for next smaller m:
               prod_{m-1} = prod_m * (2*m) * (2*m-1)   */
            prod = prod * (seq_t)(2 * m) * (seq_t)(2 * m - 1);
        }
    }

    return sum;
}
