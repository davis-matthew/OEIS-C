/*
 * A160609  Numerator of Laguerre(n, -5).
 * https://oeis.org/A160609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Laguerre polynomial L_n(x) evaluated at x =
 *            -5. Using the recurrence: (n+1)L_{n+1}(-5) = (2n+6)L_n(-5) -
 *            nL_{n-1}(-5)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* U_{n-2} = L_{n-2}(-5), U_{n-1} = L_{n-1}(-5) as rational num/den */
    seq_t num_nm2 = (seq_t)1, den_nm2 = (seq_t)1;   /* L_0 = 1 / 1      */
    seq_t num_nm1 = (seq_t)6, den_nm1 = (seq_t)1;   /* L_1 = 6 / 1      */

    for (idx_t i = 1; i < n; ++i)  /* compute L_{i+1} from L_i and L_{i-1} */
    {
        /* (2i+6)*L_i */
        seq_t coeff1 = (seq_t)(2 * (seq_t)i + 6);
        seq_t a1 = coeff1 * num_nm1;
        seq_t d1 = den_nm1;

        /* i*L_{i-1} */
        seq_t a2 = (seq_t)i * num_nm2;
        seq_t d2 = den_nm2;

        /* Difference a1/d1 - a2/d2 with common denominator */
        seq_t g = gcd(d1, d2);
        seq_t lcm = (d1 / g) * d2;           /* lcm of denominators   */
        seq_t part1 = a1 * (d2 / g);
        seq_t part2 = a2 * (d1 / g);
        seq_t diff_num = part1 - part2;
        seq_t diff_den = lcm;

        /* Divide by (i+1) to get U_{i+1} */
        seq_t new_num = diff_num;
        seq_t new_den = diff_den * (seq_t)(i + 1);

        /* Reduce the fraction */
        seq_t gg = gcd(new_num, new_den);
        new_num /= gg;
        new_den /= gg;

        /* Shift for next iteration */
        num_nm2 = num_nm1;   den_nm2 = den_nm1;
        num_nm1 = new_num;   den_nm1 = new_den;
    }

    /* num_nm1/den_nm1 now holds L_n(-5).  Return the numerator. */
    return num_nm1;
}
