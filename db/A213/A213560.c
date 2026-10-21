/*
 * A213560  Antidiagonal sums of the convolution array A213558.
 * https://oeis.org/A213560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)^2*(n+2)*(16+18*n+21*n^2+12*n^3+3*n^4)/840
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 31, u64 n <= 516, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    const seq_t rn = (seq_t)n;

    /* Build the five multiplicative factors */
    seq_t f1 = rn;                      /* n */
    seq_t f2 = rn + 1;                  /* n+1 */
    seq_t f3 = rn + 1;                  /* n+1 (again) */
    seq_t f4 = rn + 2;                  /* n+2 */

    /* Polynomial part: 16 + 18*n + 21*n^2 + 12*n^3 + 3*n^4 */
    seq_t f5 = 16;
    f5 += (seq_t)18 * rn;
    f5 += (seq_t)21 * rn * rn;
    f5 += (seq_t)12 * rn * rn * rn;
    f5 += (seq_t)3  * rn * rn * rn * rn;

    /* Reduce each factor with the denominator 840 to keep values small */
    int denom = 840;            /* 2^3 * 3 * 5 * 7 */
    seq_t result = 1;

    /* Process factors one by one */
    seq_t *factors[5] = { &f1, &f2, &f3, &f4, &f5 };
    for (int i = 0; i < 5; ++i)
    {
        seq_t val = *factors[i];
        seq_t g = gcd(val, (seq_t)denom);
        if (g > 1)
        {
            val /= g;
            denom /= (int)g;
        }
        result *= val;
    }

    /* After processing all factors, denom should be 1 */
    return result;
}
