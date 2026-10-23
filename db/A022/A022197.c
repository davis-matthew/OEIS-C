/*
 * A022197  Gaussian binomial coefficients [ n,6 ] for q = 3.
 * https://oeis.org/A022197
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=1..6} (3^(n-i+1)-1)/(3^i-1)
 * Range:     u8 none, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/gcd.h"

/* Computes the Gaussian binomial coefficient [n,6] for q=3.
   The sequence is defined for n ≥ 6. For n < 6 the value is
   unspecified; this implementation returns 0. */
seq_t a(idx_t n)
{
    if (n < 6)
        return (seq_t)0;   /* undefined for n < 6 */

    seq_t result = 1;

    for (idx_t i = 1; i <= 6; ++i) {
        /* exponent for the numerator and denominator */
        idx_t exp_num = n - i + 1;
        idx_t exp_den = i;

        /* compute numerator and denominator */
        seq_t num = big_pow((seq_t)3, (seq_t)exp_num) - 1;
        seq_t den = big_pow((seq_t)3, (seq_t)exp_den) - 1;

        /* cancel common factors between num and den */
        seq_t g1 = gcd(num, den);
        num /= g1;
        den /= g1;

        /* cancel common factors between current result and den */
        seq_t g2 = gcd(result, den);
        result /= g2;
        den /= g2;           /* should now be 1 */

        /* multiply result by the reduced numerator */
        result *= num;
        /* den should be 1; if not, division would truncate with loss
           but formula guarantees integrality after the above reductions */
    }

    return result;
}
