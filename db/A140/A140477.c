/*
 * A140477  Binomial(127,n).
 * https://oeis.org/A140477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(127, n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"


seq_t a(idx_t n)
{
    /* For n > 127 the binomial coefficient is zero. */
    if (n > 127)
        return 0;

    /* Use symmetry to keep k small: C(127,n) = C(127,127-n). */
    idx_t k = n;
    if (k > 127 - k)
        k = 127 - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* Numerator and denominator for this step. */
        seq_t num = (seq_t)(127 - k + i);
        seq_t den = (seq_t)i;

        /* Reduce numerator/denominator pair. */
        seq_t g = gcd(num, den);
        if (g > 1)
        {
            num /= g;
            den /= g;
        }

        /* Cancel any remaining denominator with the partial result. */
        g = gcd(res, den);
        if (g > 1)
        {
            res /= g;
            den /= g;
        }

        /* Multiply by the simplified numerator. */
        res *= num;
        if (den != 1)
            res /= den;
    }

    return res;
}
