/*
 * A014142  Inverse of 133rd cyclotomic polynomial.
 * https://oeis.org/A014142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the Laurent expansion of
 *            1/Φ133(x). The sequence is periodic with period 133
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define DEG_N    134      /* degree of (x – 1)(x^133 – 1)      */
#define DEG_D    26       /* degree of (x^7 – 1)(x^19 – 1)     */
#define DEG_Q    108      /* degree of Φ133(x)                  */
#define PERIOD   133      /* period length                       */

seq_t a(idx_t n)
{
    static bool built = false;
    static seq_t coeff[DEG_Q + 1];
    static seq_t cache[PERIOD];

    if (!built)
    {
        built = true;

        /* Build Φ133: N(x) / D(x)  where
           N(x) = (x‑1)(x^133‑1) = x^134 – x^133 – x + 1
           D(x) = (x^7‑1)(x^19‑1) = x^26 – x^19 – x^7 + 1
        */
        seq_t N[DEG_N + 1] = {0};
        seq_t D[DEG_D + 1] = {0};
        seq_t Q[DEG_Q + 1] = {0};

        /* numerator */
        N[134] =  1;
        N[133] = -1;
        N[1]   = -1;
        N[0]   =  1;

        /* denominator */
        D[26] =  1;
        D[19] = -1;
        D[7]  = -1;
        D[0]  =  1;

        /* long division N / D */
        int degN_cur = DEG_N;
        while (degN_cur >= DEG_D)
        {
            int k = degN_cur - DEG_D;
            seq_t factor = N[degN_cur];       /* leading coefficient of N */
            Q[k] = factor;                    /* store quotient coefficient */

            for (int i = 0; i <= DEG_D; ++i)
            {
                N[degN_cur - i] -= factor * D[DEG_D - i];
            }

            while (degN_cur >= 0 && N[degN_cur] == 0)
                --degN_cur;
        }

        /* copy quotient into coefficient array */
        for (int i = 0; i <= DEG_Q; ++i)
            coeff[i] = Q[i];

        /* compute one full period of a(n) using the linear recurrence
           a(n) = – Σ_{i=1..108} coeff[i] · a(n–i)  for n ≥ 1
        */
        cache[0] = 1;                       /* given a(0) = 1 */

        for (int i = 1; i < PERIOD; ++i)
        {
            seq_t sum = 0;
            int max = (i < DEG_Q) ? i : DEG_Q;
            for (int j = 1; j <= max; ++j)
                sum += coeff[j] * cache[i - j];
            cache[i] = -sum;
        }
    }

    return cache[n % PERIOD];
}
