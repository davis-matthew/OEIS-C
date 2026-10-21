/*
 * A059603  Expansion of (1+15*x+15*x^2+x^3)/(1-x)^12.
 * https://oeis.org/A059603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+8,8)*(2*n+9)*(8*n^2+72*n+55)/(11*5*9)
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 197, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Denominator = 8! * 495 = 19958400 */
    const seq_t DENOM = 19958400;
    seq_t result = 1;
    seq_t denom  = DENOM;

    /* Multiply by (n+1)...(n+8) and reduce with denominator */
    for (idx_t k = 1; k <= 8; ++k) {
        result *= ((seq_t)n + k);
        seq_t g = gcd(result, denom);
        if (g > 1) {
            result /= g;
            denom  /= g;
        }
    }

    /* Multiply by (2*n+9) and reduce */
    result *= ((seq_t)2 * n + 9);
    seq_t g = gcd(result, denom);
    if (g > 1) {
        result /= g;
        denom  /= g;
    }

    /* Multiply by (8*n^2 + 72*n + 55) and reduce */
    seq_t t3 = (seq_t)8 * n * n + (seq_t)72 * n + 55;
    result *= t3;
    g = gcd(result, denom);
    if (g > 1) {
        result /= g;
        denom  /= g;
    }

    /* At this point denom should be 1; return the result */
    return result;
}
