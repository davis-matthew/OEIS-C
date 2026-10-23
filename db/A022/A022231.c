/*
 * A022231  Gaussian binomial coefficients [ n,2 ] for q = 7.
 * https://oeis.org/A022231
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7^(n-1)-1)*(7^n-1)/288
 * Range:     u8 none, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n ≥ 2 with offset 2 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Compute 7^(n-1) */
    seq_t pow_n_minus1 = 1;
    for (idx_t i = 0; i < n - 1; ++i)
        pow_n_minus1 *= (seq_t)7;

    /* Compute 7^n = 7 * 7^(n-1) */
    seq_t pow_n = pow_n_minus1 * (seq_t)7;

    /* A = 7^(n-1)-1, B = 7^n-1 */
    seq_t A = pow_n_minus1 - (seq_t)1;
    seq_t B = pow_n - (seq_t)1;

    /* Reduce by the divisor 288 to avoid overflow on intermediate products */
    seq_t d = 288;
    while (d > 1)
    {
        seq_t g = gcd(A, d);
        if (g > 1)
        {
            A /= g;
            d /= g;
        }
        else
        {
            g = gcd(B, d);
            if (g > 1)
            {
                B /= g;
                d /= g;
            }
            else
            {
                /* Should not happen for valid inputs */
                break;
            }
        }
    }

    /* After division, the product A*B equals the desired value */
    return A * B;
}
