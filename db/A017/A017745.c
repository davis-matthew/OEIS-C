/*
 * A017745  Binomial coefficients C(n,81).
 * https://oeis.org/A017745
 *
 * Offset:    81    (a(81) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n,81) for n >= 81, a(n) = 0 for n < 81 C(n,k) is
 *            computed multiplicatively with GCD reductions to avoid overflow
 *            on 64‑bit builds
 * Range:     u8 n <= 82, u16 n <= 83, u32 n <= 87, u64 n <= 98, u128 n <= 135,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 81)
        return 0;

    const idx_t K = 81;          /* fixed binomial order */
    seq_t result = 1;            /* C(n,0) */

    for (idx_t i = 1; i <= K; ++i)
    {
        /* Step: result = result * (n-K+i) / i  with intermediate cancellations */
        seq_t num = (seq_t)(n - K + i); /* numerator factor */
        seq_t den = (seq_t)i;           /* denominator factor */

        /* cancel common factors between current result and denominator */
        seq_t g = gcd(result, den);
        result /= g;
        den /= g;

        /* further cancel between numerator and remaining denominator */
        g = gcd(num, den);
        num /= g;
        den /= g;

        /* multiply by reduced numerator */
        result *= num;

        /* if any denominator factor remains, divide (should be 1 on exact division) */
        if (den > 1)
            result /= den;
    }

    return result;
}
