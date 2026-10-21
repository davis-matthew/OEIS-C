/*
 * A228999  Total sum of the 9th powers of lengths of ascending runs in all
 *          permutations of [n].
 * https://oeis.org/A228999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t=1}^{n-1} t^9 * * ( 2t * (n! / (t+1)!) + (n - t -
 *            1)(t^2 + t - 1) * (n! / (t+2)!) ) * + n^9 * This sum gives the
 *            total sum of the 9th powers of lengths of ascending runs * in
 *            all permutations of [n]
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 0;

    /* n^9 term (run of length n, which occurs exactly once) */
    seq_t pow_n9 = (seq_t)n;
    for (idx_t i = 1; i < 9; ++i) pow_n9 *= (seq_t)n;

    /* r = product_{k=t+2}^{n} k  ( = n!/(t+1)! ), s = product_{k=t+3}^{n} k ( = n!/(t+2)! ) */
    seq_t r = 1;   /* valid for t = n-1 */
    seq_t s = 1;   /* valid for t = n-2 (but used only when t <= n-2) */

    for (idx_t t = n-1; t > 0; --t) /* descending t */
    {
        /* t^9 */
        seq_t pow_t9 = (seq_t)t;
        for (idx_t i = 1; i < 9; ++i) pow_t9 *= (seq_t)t;

        /* term1 = 2t * r */
        seq_t term1 = (seq_t)(2 * t) * r;

        /* term2 = (n - t - 1)(t^2 + t - 1) * s , only when t <= n-2 */
        seq_t term2 = 0;
        if (t <= n-2) {
            idx_t coeff = n - t - 1;
            idx_t tmp   = t*t + t - 1;        /* small, fits in idx_t */
            term2 = (seq_t)coeff * (seq_t)tmp * s;
        }

        /* R(n,t) = term1 + term2 */
        seq_t runCount = term1 + term2;

        /* add contribution of all runs of length t */
        result += runCount * pow_t9;

        /* update r and s for the next smaller t */
        if (t > 1) {
            r = r * (seq_t)(t + 1);
            if (t <= n-2) {          /* s needed only when next t <= n-2 */
                s = s * (seq_t)(t + 2);
            }
        }
    }

    /* add the t = n contribution */
    result += pow_n9;

    return result;
}
