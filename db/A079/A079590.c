/*
 * A079590  a(n) = C(6*n+1,n).
 * https://oeis.org/A079590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(6*n+1,n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* numerator = 5*n + k + 1  ( values 5n+2 … 6n+1 ) */
        seq_t numerator = (seq_t)(5 * n) + (seq_t)1 + (seq_t)k;
        seq_t denom = (seq_t)k;
        seq_t g;

        /* cancel common factors with result */
        g = gcd(result, denom);
        result /= g;
        denom  /= g;

        /* cancel remaining factors from denom with numerator */
        g = gcd(numerator, denom);
        numerator /= g;
        denom  /= g;   /* denom should now be 1 */

        /* multiply the reduced numerator */
        result *= numerator;
    }
    return result;
}
