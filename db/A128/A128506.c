/*
 * A128506  Numerators of partial sums for a series for 3*sqrt(2)*(Pi^3)/2^7.
 * https://oeis.org/A128506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of r(n) where r(n) = Σ_{k=0..n}
 *            S(2k,√2)/(2k+1)^3, S(2k,√2) = 1,1,-1,-1 repeating (period 4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 0;   /* current numerator   */
    seq_t den = 1;   /* current denominator (reduced) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* sign pattern: 1,1,-1,-1 repeating every 4 */
        int sign = (k & 2) ? -1 : 1;      /* 0,1 -> +1 ; 2,3 -> -1 */

        /* denominator (2k+1)^3 */
        seq_t odd  = ((seq_t)2 * k) + 1;
        seq_t denom = odd * odd * odd;

        /* reduce and combine fractions */
        seq_t g  = gcd(den, denom);          /* gcd(den, denom) */
        seq_t denom_div_g = denom / g;       /* denom/g   */
        seq_t term = den / g;                /* den/g     */
        seq_t prod = num * denom_div_g;      /* num*(den/g) */

        seq_t new_num;
        if (sign == 1)
            new_num = prod + term;
        else
            new_num = prod - term;           /* safe: prod >= term */

        seq_t new_den = den * denom_div_g;   /* lcm(den, denom) */

        /* reduce the resulting fraction again */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return num;   /* the reduced numerator of r(n) */
}
