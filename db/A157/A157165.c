/*
 * A157165  Numerators of partial sums of a series related to Lebesgue's
 *          constant L(0) = 1.
 * https://oeis.org/A157165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator(3*Sum_{k=1..n} Theta(1,k)/(4*k^2-1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with Theta(1,k) = Sum_{j=1..k} 1/(2*j-1)                       */
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(1) should give 1; the loop works for any n>=1                */
    if (n == 0) return 0;   /* not used, but harmless */

    seq_t sum_num = 0;      /* numerator of the accumulated sum      */
    seq_t sum_den = 1;      /* denominator of the accumulated sum    */
    seq_t theta_num = 0;    /* numerator of Theta(1,k)               */
    seq_t theta_den = 1;    /* denominator of Theta(1,k)             */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* update Theta(1,k) = Theta(1,k-1) + 1/(2*k-1)                */
        seq_t odd = (seq_t)2 * k - 1;          /* (2k-1)                      */
        seq_t new_theta_num = theta_num * odd + theta_den;
        seq_t new_theta_den = theta_den * odd;
        seq_t g = gcd(new_theta_num, new_theta_den);
        theta_num = new_theta_num / g;
        theta_den = new_theta_den / g;

        /* term t(k) = 3 * Theta / (4*k^2-1)                           */
        seq_t denom_factor = (seq_t)4 * k * k - 1; /* 4k^2-1           */
        seq_t t_num = 3 * theta_num;
        seq_t t_den = theta_den * denom_factor;
        g = gcd(t_num, t_den);
        t_num /= g;
        t_den /= g;

        /* add t(k) to the accumulated sum                           */
        seq_t g1 = gcd(sum_den, t_den);
        seq_t sum_den_red = sum_den / g1;
        seq_t t_den_red   = t_den   / g1;

        seq_t tmp1 = sum_num * t_den_red;   /* a*t'           */
        seq_t tmp2 = t_num   * sum_den_red; /* b*s'           */
        seq_t num_add = tmp1 + tmp2;
        seq_t den_add = sum_den_red * t_den; /* s'*t           */

        g = gcd(num_add, den_add);
        sum_num = num_add / g;
        sum_den = den_add / g;
    }

    return sum_num;   /* the fraction is kept in lowest terms */
}
