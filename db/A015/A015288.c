/*
 * A015288  Gaussian binomial coefficient [ n,4 ] for q = -3.
 * https://oeis.org/A015288
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,4] for q = -3 a(n) =
 *            Π_{i=0}^{3} (1 - (-3)^{n-i}) / (1 - (-3)^{i+1}) (n ≥ 4)
 * Range:     u8 n <= 4, u16 none, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   seq_gcd_internal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_gcd_internal.h"

seq_t a(idx_t n)
{
    /* Undefined behavior for n < 4; return 0. */
    if (n < 4)
        return (seq_t)0;

    /* Initial denominator: 4 * (-8) * 28 * (-80) = 71680. */
    seq_t denom = (seq_t)71680;
    seq_t prod  = (seq_t)1;

    /* Compute 3^(n-3). */
    seq_t t = (seq_t)1;
    for (idx_t i = 0; i < n - 3; ++i)
        t *= (seq_t)3;

    /* Iterate through j = n-3 .. n. */
    for (idx_t j = n - 3; j <= n; ++j)
    {
        /* Numerator factor: 1 - (-3)^j.  If j is even, this equals 3^j - 1;
           if j is odd, it equals 3^j + 1. */
        seq_t v;
        if ((j & (idx_t)1) == 0)            /* j even */
            v = t - (seq_t)1;
        else                                 /* j odd */
            v = t + (seq_t)1;

        /* Cancel common factors with the denominator. */
        seq_t g = seq_gcd_internal(v, denom);
        if (g > 1) {
            denom /= g;
            v /= g;
        }

        prod *= v;

        /* Prepare t for the next j (multiply by 3). */
        t *= (seq_t)3;
    }

    /* After the loop denom should be 1; no need to check. */
    return prod;
}
