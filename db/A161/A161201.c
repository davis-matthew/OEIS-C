/*
 * A161201  Numerators in expansion of (1-x)^(-7/2).
 * https://oeis.org/A161201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = numerator( (-1)^n * binomial(-7/2 , n) )
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t num = 1;   /* numerator of the current fraction */
    seq_t den = 1;   /* denominator of the current fraction */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t mult_num = (seq_t)2 * (seq_t)k + 5;   /* 2k + 5 */
        seq_t mult_den = (seq_t)2 * (seq_t)k;       /* 2k */

        /* Reduce the multiplier fraction */
        seq_t g1 = gcd(mult_num, mult_den);
        if (g1 > 1) {
            mult_num /= g1;
            mult_den /= g1;
        }

        /* Cancel cross‑factors with the current numerator */
        seq_t g2 = gcd(num, mult_den);
        if (g2 > 1) {
            num /= g2;
            mult_den /= g2;
        }

        /* Cancel cross‑factors with the current denominator */
        seq_t g3 = gcd(den, mult_num);
        if (g3 > 1) {
            mult_num /= g3;
            den /= g3;
        }

        /* Multiply the current fraction by the reduced multiplier */
        num *= mult_num;
        den *= mult_den;

        /* Final reduction of the resulting fraction */
        seq_t g4 = gcd(num, den);
        if (g4 > 1) {
            num /= g4;
            den /= g4;
        }
    }

    return num;
}
