/*
 * A169959  a(n) = binomial(10*n, n).
 * https://oeis.org/A169959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(10*n, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute binomial coefficient C(10*n, n) by multiplicative formula
 * with GCD cancellation to avoid intermediate overflow on 64-bit types.
 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return 1;

    seq_t result = 1;

    for (idx_t i = 1; i <= n; ++i) {
        /* num = 9*n + i, den = i */
        seq_t num = (seq_t)9 * (seq_t)n + (seq_t)i;
        seq_t den = (seq_t)i;

        /* cancel common factors between numerator and denominator */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* cancel common factors between current result and remaining denominator */
        g = gcd(result, den);
        result /= g;
        den /= g;   /* den should now be 1 */

        /* multiply and divide by any remaining denominator (should be 1) */
        result *= num;
        if (den != 1)
            result /= den;
    }

    return result;
}
