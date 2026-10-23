/*
 * A022196  Gaussian binomial coefficients [ n,5 ] for q = 3.
 * https://oeis.org/A022196
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=1..5} (3^(n-i+1)-1)/(3^i-1)
 * Range:     u8 n <= 5, u16 none, u32 n <= 8, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;

    /* Denominator product ∏_{i=1..5}(3^i-1) = 8053760 */
    seq_t denom = (seq_t)8053760;
    seq_t result = 1;

    for (idx_t i = 1; i <= 5; ++i) {
        idx_t exp = n - i + 1;
        seq_t num = big_pow((seq_t)3, (seq_t)exp) - 1;

        /* Reduce the numerator with the remaining part of the denominator */
        seq_t g = gcd(num, denom);
        if (g > 1) {
            num = num / g;
            denom = denom / g;
        }

        result = result * num;

        /* Reduce the running result with whatever denominator remains */
        if (denom > 1) {
            seq_t g2 = gcd(result, denom);
            if (g2 > 1) {
                result = result / g2;
                denom = denom / g2;
            }
        }
    }

    return result;
}
