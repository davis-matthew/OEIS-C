/*
 * A014230  Inverse of 221st cyclotomic polynomial.
 * https://oeis.org/A014230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the n‑th coefficient of the formal power series
 *            1/Φ_221(x), where Φ_221(x) = (x‑1)(x^221‑1)/((x^13‑1)(x^17‑1)).
 *            The sequence is periodic with period 221 and satisfies a(0) = 1,
 *            a(n) = -∑_{k=1}^{min(n,192)} coeff_k * a(n‑k) (n ≥ 1), with
 *            coeff_k the coefficients of Φ_221(x) (coeff_0 = 1)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static data built once */
    static bool ready = false;
    static seq_t a_tab[221];
    static int32_t coeff[193];          /* coeff[0] = 1, coeff[1..192] for Φ_221(x) */

    if (!ready)
    {
        /* Numerator N(x) = (x-1)(x^221-1) = x^222 - x^221 - x + 1 */
        int32_t num[223] = {0};
        num[0] =  1;          /* +1       */
        num[1] = -1;          /* -x       */
        num[221] = -1;        /* -x^221   */
        num[222] =  1;        /* +x^222   */

        /* Denominator D(x) = (x^13-1)(x^17-1) = x^30 - x^13 - x^17 + 1 */
        int32_t den[31] = {0};
        den[0] =  1;          /* +1       */
        den[13] = -1;         /* -x^13    */
        den[17] = -1;         /* -x^17    */
        den[30] =  1;         /* +x^30    */

        /* Long division N(x)/D(x) → Φ_221(x) (degree 192) */
        int32_t rem[223];
        for (int i = 0; i <= 222; ++i) rem[i] = num[i];
        int32_t quot[193] = {0};

        const int deg_num = 222;
        const int deg_den = 30;
        const int deg_res = deg_num - deg_den;   /* 192 */

        for (int i = deg_num; i >= deg_den; --i)
        {
            int32_t factor = rem[i];
            int shift = i - deg_den;
            quot[shift] = factor;

            for (int j = 0; j <= deg_den; ++j)
                rem[shift + j] -= factor * den[j];
        }

        /* Store polynomial coefficients */
        coeff[0] = 1;  /* constant term of Φ_221(x) */
        for (int i = 1; i <= deg_res; ++i)
            coeff[i] = quot[i];

        /* Compute first 221 inverse series coefficients */
        a_tab[0] = 1;
        for (int idx = 1; idx < 221; ++idx)
        {
            seq_t sum = 0;
            int limit = (idx < 192) ? idx : 192;
            for (int k = 1; k <= limit; ++k)
            {
                int32_t ck = coeff[k];
                if (ck)
                    sum += (seq_t)ck * a_tab[idx - k];
            }
            a_tab[idx] = -sum;
        }

        ready = true;
    }

    idx_t m = n % 221;
    return a_tab[(size_t)m];
}
