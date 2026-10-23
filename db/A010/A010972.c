/*
 * A010972  a(n) = binomial(n,19).
 * https://oeis.org/A010972
 *
 * Offset:    19    (a(19) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,19)
 * Range:     u8 n <= 20, u16 none, u32 n <= 34, u64 n <= 91, u128 n <= 854,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/*
 * Computes a(n) = binomial(n,19) for n >= 19.
 * Uses multiplicative formula with GCD reductions to keep intermediate
 * values within bounds.
 */
seq_t a(idx_t n)
{
    if (n < 19)
        return 0;        /* undefined for n < 19; caller should not request */

    seq_t result = 1;
    for (idx_t i = 1; i <= 19; ++i) {
        /* numerator = n - i + 1, denominator = i */
        seq_t num = (seq_t)(n - i + 1);
        seq_t den = (seq_t)i;

        /* cancel common factors between num and den */
        seq_t g = gcd(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        }

        /* cancel remaining factors in den with current result */
        seq_t g2 = gcd(result, den);
        if (g2 > 1) {
            result /= g2;
            den /= g2;
        }

        /* after cancellations den should be 1
           multiply result by the reduced numerator */
        result = result * num;
    }
    return result;
}
