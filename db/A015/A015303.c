/*
 * A015303  Gaussian binomial coefficient [ n,4 ] for q = -13.
 * https://oeis.org/A015303
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=1..4} ((-13)^(n-i+1)-1)/((-13)^i-1)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0ULL;          /* offset not defined for n<4 */

    seq_t num[4];
    seq_t den[4];

    /* compute numerators and denominators (absolute values) */
    for (idx_t i = 1; i <= 4; ++i) {
        idx_t k = n - i + 1;          /* exponent for the numerator */

        /* 13^k */
        seq_t pow_num = 1;
        for (idx_t t = 0; t < k; ++t) pow_num *= (seq_t)13;

        /* abs(numerator) = |(-13)^k - 1| */
        if (k & 1u)                    /* odd: -(13^k) - 1  => abs = 13^k + 1 */
            num[i-1] = pow_num + (seq_t)1;
        else                           /* even:  13^k - 1          */
            num[i-1] = pow_num - (seq_t)1;

        /* 13^i for the denominator */
        seq_t pow_den = 1;
        for (idx_t t = 0; t < i; ++t) pow_den *= (seq_t)13;

        /* abs(denominator) = |(-13)^i - 1| */
        if (i & 1u)                    /* odd: -(13^i) - 1  => abs = 13^i + 1 */
            den[i-1] = pow_den + (seq_t)1;
        else                           /* even:  13^i - 1          */
            den[i-1] = pow_den - (seq_t)1;
    }

    /* cancel common factors between numerators and denominators */
    for (idx_t di = 0; di < 4; ++di) {
        seq_t d = den[di];
        if (d == 1) continue;
        for (idx_t ni = 0; ni < 4 && d > 1; ++ni) {
            seq_t g = gcd(num[ni], d);
            if (g > 1) {
                num[ni] /= g;
                d     /= g;
            }
        }
        den[di] = d;     /* should be 1 after full cancellation */
    }

    /* multiply the remaining numerator factors */
    seq_t result = 1;
    for (idx_t i = 0; i < 4; ++i)
        result *= num[i];

    return result;
}
