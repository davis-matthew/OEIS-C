/*
 * A017754  Binomial coefficients C(n,90).
 * https://oeis.org/A017754
 *
 * Offset:    90    (a(90) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n,90) (i.e., C(n,90))
 * Range:     u8 n <= 91, u16 n <= 92, u32 n <= 96, u64 n <= 106,
 *            u128 n <= 140, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Values are defined only for n >= 90; below the offset the behavior
       does not matter. */
    if (n < 90)
        return 0;

    const idx_t k = 90;
    seq_t result = 1;

    for (idx_t i = 1; i <= k; ++i)
    {
        /* (n‑k+i) is the ith factor of the numerator. */
        seq_t num   = (seq_t)(n - k + i);
        seq_t denom = (seq_t)i;

        /* Cancel common factors between num and denom. */
        seq_t g = gcd(num, denom);
        num   /= g;
        denom /= g;

        /* Cancel common factors between result and the remaining denom. */
        g = gcd(result, denom);
        result /= g;
        denom  /= g;

        /* Multiply and divide – the division is exact. */
        result = result * num;
        result = result / denom;
    }

    return result;
}
