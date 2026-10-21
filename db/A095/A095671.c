/*
 * A095671  Ninth column (m=8) of (1,4)-Pascal triangle A095666.
 * https://oeis.org/A095671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+32)*C(n+7,7)/8 (n>=0) Derived from the OEIS entry for
 *            A095671
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 53, u64 n <= 956, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Numerators: n+32 and the 7 terms of C(n+7,7) */
    seq_t numerators[8];
    numerators[0] = (seq_t)(n + 32);
    for (idx_t i = 1; i < 8; ++i)
        numerators[i] = (seq_t)(n + 8 - i);   /* n+7, n+6, … , n+1 */

    /* Denominator factors: 8 from the division and 7! from the binomial */
    seq_t denom[8] = { 8,7,6,5,4,3,2,1 };

    seq_t res = 1;

    /* Multiply numerators, canceling with denominators on the fly */
    for (idx_t i = 0; i < 8; ++i)
    {
        seq_t t = numerators[i];

        for (idx_t j = 0; j < 8; ++j)
        {
            seq_t d = denom[j];
            if (d == 1) continue;
            seq_t g = gcd(t, d);
            if (g > 1)
            {
                t /= g;
                denom[j] = d / g;
            }
        }

        res *= t;
    }

    /* Any remaining denominator factors (should all be 1) divide the result */
    for (idx_t j = 0; j < 8; ++j)
        if (denom[j] != 1)
            res /= denom[j];

    return res;
}
