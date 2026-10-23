/*
 * A028613  Expansion of theta_3(q) * theta_3(q^12) + theta_2(q) *
 *          theta_2(q^12) in powers of q^(1/4).
 * https://oeis.org/A028613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A028613:
 *
 *  a(n) = #{ (k,m) ∈ ℤ² | 4k² + 48m² = n } +
 *         #{ (k,m) ∈ ℤ² | 4k² + 48m² + 4k + 48m + 13 = n }
 *
 *  (counting the two separate summations of the theta function product).
 *
 *  The formula follows from
 *    theta_3(q)=∑_k q^{k²},   theta_2(q)=∑_k q^{(k+½)²},
 *  and  q = z⁴ where z is the formal variable of the series.
 *  The exponent of z in the two product terms gives the two equations
 *  above.  Counting the integer solutions yields the coefficient a(n).
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* Compute bounds for k and m that could contribute.
       For n up to 10 000 the ranges are very small. */
    long long maxk = (long long)(sqrt((double)n / 4.0)) + 10;
    long long maxm = (long long)(sqrt((double)n / 48.0)) + 10;

    for (long long k = -maxk; k <= maxk; ++k) {
        long long k2 = k * k;
        for (long long m = -maxm; m <= maxm; ++m) {
            long long m2 = m * m;
            long long e1 = 4 * k2 + 48 * m2;                /*  theta3 * theta3(q^12) term  */
            if ((seq_t)e1 == n)
                ++result;
            long long e2 = e1 + 4 * k + 48 * m + 13;         /*  theta2 * theta2(q^12) term  */
            if ((seq_t)e2 == n)
                ++result;
        }
    }
    return result;
}
