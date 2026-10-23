/*
 * A130413  Numerators of partial sums for a series for Pi/3.
 * https://oeis.org/A130413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator(r(n)) where r(n) = 1 + (4/3)*Σ_{j=1..n}
 *            (-1)^(j+1)/((2*j+1)*((2*j+1)^2-1))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include gcd utility for seq_t */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle offset */
    if (n == 0)
        return (seq_t)1;

    /* r(0) = 1/1 */
    seq_t num = (seq_t)1;   /* numerator of current r */
    seq_t den = (seq_t)1;   /* denominator of current r */

    for (idx_t j = 1; j <= n; ++j)
    {
        /* a_j = (2j+1)*((2j+1)^2-1) = 4*j*(j+1)*(2j+1) */
        seq_t two_j_plus_one = (seq_t)(2 * j + 1);
        seq_t a_val = (seq_t)4 * j * (j + 1) * two_j_plus_one;

        /* delta = (4/3) * ((-1)^(j+1))/a_j  →  delta = (±4)/(3*a_j) */
        seq_t delta_den = (seq_t)3 * a_val;                     /* denominator of delta */

        /* Reduce common factor between current denominator and delta denominator */
        seq_t g1 = gcd(den, delta_den);
        seq_t den_red = den / g1;
        seq_t delta_den_red = delta_den / g1;   /* not strictly needed but clear */

        /* Compute new numerator:
           new_num = num * delta_den_red + sign*4*den_red          */
        seq_t part1 = num * delta_den_red;

        if (j & 1)        /* odd j → positive term   */
            num = part1 + ((seq_t)4 * den_red);
        else              /* even j → negative term  */
            num = part1 - ((seq_t)4 * den_red);

        /* New denominator: lcm(den, delta_den) = den_red * delta_den */
        den = den_red * delta_den;

        /* Reduce the fraction again */
        seq_t g2 = gcd(num, den);
        if (g2 > 1)
        {
            num /= g2;
            den /= g2;
        }
    }

    return num;   /* numerator of r(n) */
}
