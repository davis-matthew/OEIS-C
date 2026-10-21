/*
 * A216938  Number of side-2 hexagonal 0..n arrays with values nondecreasing
 *          E, SW and SE.
 * https://oeis.org/A216938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+5)*(n+4)*(n+3)*(n+2)*(n+1)*(2*n^2+12*n+21)/2520
 * Range:     u8 none, u16 n <= 10, u32 n <= 62, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n==0 the value is irrelevant, return 0. */
    if (n == 0)
        return 0;

    seq_t rn = (seq_t)n;

    /* Prepare the six numerator factors. */
    seq_t terms[6];
    terms[0] = rn + (seq_t)5;
    terms[1] = rn + (seq_t)4;
    terms[2] = rn + (seq_t)3;
    terms[3] = rn + (seq_t)2;
    terms[4] = rn + (seq_t)1;
    terms[5] = (seq_t)2 * rn * rn + (seq_t)12 * rn + (seq_t)21;

    /* Reduce each factor with the denominator 2520. */
    seq_t denom = (seq_t)2520;
    for (int i = 0; i < 6; ++i)
    {
        seq_t g = gcd(terms[i], denom);
        if (g > 1)
        {
            terms[i] /= g;
            denom /= g;
        }
    }

    /* Multiply the reduced terms. */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 6; ++i)
        result *= terms[i];

    /* Any remaining divisor (should be 1) is divided last. */
    if (denom != 1)
        result /= denom;

    return result;
}
