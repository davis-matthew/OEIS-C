/*
 * A010971  a(n) = binomial(n,18).
 * https://oeis.org/A010971
 *
 * Offset:    18    (a(18) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,18)
 * Range:     u8 n <= 20, u16 n <= 23, u32 n <= 34, u64 n <= 97,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(n) is undefined for n < 18; return 0 in that case */
    if (n < 18)
        return 0;

    const idx_t K = 18;
    seq_t result = 1;

    for (idx_t i = 1; i <= K; ++i)
    {
        seq_t num = (seq_t)n - K + i;   /* (n - K + i) */
        seq_t den = (seq_t)i;          /* i */

        /* Cancel common factors between result and denominator */
        seq_t g = gcd(result, den);
        result /= g;
        den /= g;

        /* Cancel remaining common factors between numerator and denominator */
        g = gcd(num, den);
        num /= g;
        den /= g;

        /* Multiply the remaining numerator */
        result *= num;

        /* If any denominator remains, divide */
        if (den != 1)
            result /= den;
    }

    return result;
}
