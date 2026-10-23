/*
 * A175385  a(n) = numerator of Sum_{i=1..n} binomial(2n-i-1,i-1)/i.
 * https://oeis.org/A175385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{i=1..n} binomial(2n-i-1,i-1)/i
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0, return 0 */

    seq_t num = 0;
    seq_t den = 1;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* term = C(2n-i-1, i-1) / i */
        seq_t term_num = binomial(2 * n - i - 1, i - 1);
        seq_t term_den = (seq_t)i;

        /* add term to the accumulating fraction num/den */
        seq_t g = gcd(den, term_den);          /* reduce the common part of denominators */
        seq_t den_div_g = den / g;
        seq_t term_den_div_g = term_den / g;   /* lcm factor for the denominators */

        /* compute new numerator and denominator */
        seq_t new_num = num * term_den_div_g + term_num * den_div_g;
        seq_t new_den = den * term_den_div_g;

        /* reduce the resulting fraction */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return num;   /* numerator of the reduced sum */
}
