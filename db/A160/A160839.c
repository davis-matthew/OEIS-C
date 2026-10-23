/*
 * A160839  Expansion of
 *          (78+1116*x+3492*x^2+3237*x^3+927*x^4+72*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0}^{6} coeff[i] * binom(n - i + 6, 6) with coeff =
 *            {78, 1116, 3492, 3237, 927, 72, 1}
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 25, u64 n <= 1067,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom6(idx_t k)
{
    /* k must be at least 6; returns C(k,6) */
    seq_t r = 1;
    /* multiply (k, k-1, …, k-5) and divide by 1…6 successively */
    for (int i = 1; i <= 6; ++i)
    {
        r = r * (seq_t)(k + 1 - i) / (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    static const seq_t coeff[7] = {
        (seq_t)78,  (seq_t)1116, (seq_t)3492,
        (seq_t)3237, (seq_t)927,  (seq_t)72,  (seq_t)1
    };

    seq_t res = 0;
    idx_t max_i = 6;
    if (n < max_i)
        max_i = n;

    for (idx_t i = 0; i <= max_i; ++i)
    {
        /* n - i + 6 is always ≥ 6 for i ≤ n, so safe to call binom6 */
        res += coeff[i] * binom6(n - i + 6);
    }

    return res;
}
