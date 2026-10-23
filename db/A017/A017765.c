/*
 * A017765  Binomial coefficients C(49,n).
 * https://oeis.org/A017765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(49,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n > 49)                          /* out of range binomial coefficient */
        return 0;

    idx_t k = n;
    if (k > 49 - n)                     /* use symmetry */
        k = 49 - n;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(49 - k + i); /* numerator factor */
        seq_t den = (seq_t)i;            /* denominator factor */

        seq_t g = gcd(num, den);         /* reduce num/den */
        num /= g;
        den /= g;

        g = gcd(res, den);               /* reduce res/den */
        res /= g;
        den /= g;

        res *= num;                      /* multiply by reduced numerator */
        if (den != 1)                    /* final safety check */
            res /= den;
    }
    return res;
}
