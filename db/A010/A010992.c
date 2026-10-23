/*
 * A010992  Binomial coefficient C(n,39).
 * https://oeis.org/A010992
 *
 * Offset:    39    (a(39) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,39) = Π_{i=1}^{39} (n-39+i)/i
 * Range:     u8 n <= 40, u16 n <= 42, u32 n <= 48, u64 n <= 68, u128 n <= 169,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 39) /* below the offset – value is irrelevant */
        return 0;

    const idx_t k = 39;
    seq_t res = 1;

    for (idx_t i = 1; i <= k; ++i) {
        idx_t num_idx = n - k + i;          /* integer factor in the numerator */
        seq_t num = (seq_t)num_idx;
        seq_t denom = (seq_t)i;

        /* cancel common factors inside the fraction */
        seq_t g = gcd(num, denom);
        num /= g;
        denom /= g;

        /* cancel common factors with the accumulated result */
        g = gcd(res, denom);
        res /= g;
        denom /= g;                      /* denom is now 1 */

        res *= num;                      /* exact multiplication */
        if (denom != 1)                  /* safety check – should never happen */
            res /= denom;
    }

    return res;
}
