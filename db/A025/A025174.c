/*
 * A025174  a(n) = binomial(3n-1, n-1).
 * https://oeis.org/A025174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n-1, n-1) (n ≥ 1, with a(0)=0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) is defined to be 0 in the OEIS data. */
    if (n == 0)
        return (seq_t)0;

    /* Parameters for the binomial coefficient C(3n-1, n-1). */
    idx_t N = 3 * n - 1;          /* upper index */
    idx_t K = n - 1;              /* lower index */
    idx_t r = (K <= N - K) ? K : N - K;  /* compute the smaller of K and N-K */

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= r; ++i)
    {
        /* Current multiplicative factors.   */
        seq_t num = (seq_t)(N - r + i);  /* numerator term */
        seq_t den = (seq_t)i;            /* denominator term */

        /* Cancel any common factor between num and den. */
        seq_t g = gcd(num, den);
        if (g > (seq_t)1) { num /= g; den /= g; }

        /* Cancel any common factor between res and den. */
        g = gcd(res, den);
        if (g > (seq_t)1) { res /= g; den /= g; }

        /* After the cancellations above, den should divide res * num
           exactly.  Multiply and then divide if needed. */
        res = res * num;
        if (den != (seq_t)1)
            res = res / den;
    }

    return res;
}
