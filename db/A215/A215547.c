/*
 * A215547  Number of standard Young tableaux of shape [7n,7].
 * https://oeis.org/A215547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(7*n+7,7)*(7*n-6)/(7*n+1) Equivalent closed form:
 *            a(n) =
 *            ((7*n+7)*(7*n+6)*(7*n+5)*(7*n+4)*(7*n+3)*(7*n+2)*(7*n-6))/5040
 *            for n > 0, a(0) = 0
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 11, u64 n <= 272, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Pre‑compute the factors in the numerator and the denominator   */
    idx_t seven_n   = n * 7U;          /* 7*n                          */
    idx_t factors[7] = { seven_n + 7,   /* 7*n+7                        */
                          seven_n + 6,   /* 7*n+6                        */
                          seven_n + 5,   /* 7*n+5                        */
                          seven_n + 4,   /* 7*n+4                        */
                          seven_n + 3,   /* 7*n+3                        */
                          seven_n + 2,   /* 7*n+2                        */
                          seven_n - 6 }; /* 7*n-6                        */

    /* The denominator 5040 = 2^4 * 3^2 * 5 * 7   */
    const seq_t den[8] = { (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2,
                           (seq_t)3, (seq_t)3, (seq_t)5, (seq_t)7 };
    seq_t remaining[8];
    for (int i = 0; i < 8; ++i)
        remaining[i] = den[i];

    seq_t result = (seq_t)1;

    for (int k = 0; k < 7; ++k)
    {
        result *= (seq_t)factors[k];          /* multiply by a numerator factor */

        /* Reduce any common factor with the remaining denominator */
        for (int i = 0; i < 8; ++i)
        {
            if (remaining[i] > (seq_t)1)
            {
                seq_t g = gcd(result, remaining[i]);
                if (g > (seq_t)1)
                {
                    result /= g;
                    remaining[i] /= g;
                }
            }
        }
    }

    /* Any leftover denominator factor (should be 1) is divided now */
    for (int i = 0; i < 8; ++i)
        if (remaining[i] > (seq_t)1)
            result /= remaining[i];

    return result;
}
