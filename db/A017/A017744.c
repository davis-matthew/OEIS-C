/*
 * A017744  Binomial coefficients C(n,80).
 * https://oeis.org/A017744
 *
 * Offset:    80    (a(80) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,80)
 * Range:     u8 n <= 81, u16 n <= 82, u32 n <= 86, u64 n <= 97, u128 n <= 135,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 80, return 0 as a placeholder */
    if (n < 80U)
        return 0;

    const idx_t K = 80U;
    seq_t res = 1;

    for (idx_t i = 1; i <= K; ++i)
    {
        /* Numerator and denominator of the current factor */
        seq_t num = n - K + i;  /* equals n - 79 + (i-1) */
        seq_t den = i;

        /* Reduce res and den by their GCD to keep numbers small */
        seq_t g = gcd(res, den);
        res /= g;
        den /= g;

        /* Reduce num and den further */
        g = gcd(num, den);
        num /= g;
        den /= g;  /* den should now be 1 */

        /* Multiply and divide */
        res = res * num;
        if (den != 1)
            res /= den;
    }

    return res;
}
