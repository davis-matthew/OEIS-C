/*
 * A204265  Permanent of n X n matrix M with M(i,j) = (i+j) (mod 3).
 * https://oeis.org/A204265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(3*m) = (m!)^3 * Sum_{i=0..m} binomial(m,i)^3 * 2^(3*m-3*i);
 *            a(3*m+1) = (m!)^2*(m+1)! * Sum_{i=0..m}
 *            binomial(m,i)^2*binomial(m+1,i) * 2^(3*m+1-3*i); a(3*m+2) =
 *            m!*(m+1)!^2 * Sum_{i=0..m}
 *            binomial(m,i)*binomial(m+1,i)*binomial(m+1,i+1) * 2^(3*m+1-3*i);
 *            (using factorial, binomial and 2^x function...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t m = n / 3;
    idx_t r = n % 3;

    seq_t fact_m  = factorial(m);
    seq_t fact_mp1 = factorial(m + 1);
    seq_t base, sum = 0;
    seq_t bin_m_i, bin_mp1_i, bin_mp1_ip1;
    seq_t term;

    switch (r)
    {
        case 0: /* n = 3*m */
            base = fact_m * fact_m * fact_m;   /* (m!)^3 */
            for (idx_t i = 0; i <= m; ++i)
            {
                bin_m_i  = binomial(m, i);
                term = bin_m_i * bin_m_i * bin_m_i;           /* binomial^3 */
                term = term * pow2(3 * (m - i));              /* * 2^(3m-3i) */
                sum = sum + term;
            }
            break;

        case 1: /* n = 3*m + 1 */
            base = fact_m * fact_m * fact_mp1;   /* (m!)^2 * (m+1)! */
            for (idx_t i = 0; i <= m; ++i)
            {
                bin_m_i  = binomial(m, i);
                bin_mp1_i = binomial(m + 1, i);
                term = bin_m_i * bin_m_i;          /* binomial(m,i)^2 */
                term = term * bin_mp1_i;           /* * binomial(m+1,i) */
                term = term * pow2(3 * (m - i) + 1); /* * 2^(3m+1-3i) */
                sum = sum + term;
            }
            break;

        case 2: /* n = 3*m + 2 */
            base = fact_m * fact_mp1 * fact_mp1;   /* m! * (m+1)!^2 */
            for (idx_t i = 0; i <= m; ++i)
            {
                bin_m_i  = binomial(m, i);
                bin_mp1_i  = binomial(m + 1, i);
                bin_mp1_ip1 = binomial(m + 1, i + 1);
                term = bin_m_i;                    /* binomial(m,i)   */
                term = term * bin_mp1_i;           /* * binomial(m+1,i) */
                term = term * bin_mp1_ip1;         /* * binomial(m+1,i+1) */
                term = term * pow2(3 * (m - i) + 1); /* * 2^(3m+1-3i) */
                sum = sum + term;
            }
            break;
    }

    return base * sum;
}
