/*
 * A157800  Denominator of Bernoulli(n, 1/3).
 * https://oeis.org/A157800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2·3ⁿ · ∏{p | p prime, p-1 | n, p > 3 } for even n ≥ 2 *
 *            a(0) = 1, a(1) = 6, and a(n) = 3ⁿ for odd n ≥ 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   is_prime, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)          return (seq_t)1;
    if (n == 1)          return (seq_t)6;

    /* Odd n ≥ 3 */
    if (n & 1)           return pow3(n);

    /* Even n ≥ 2 */
    seq_t denom = 2;                /* factor 2 */
    denom *= pow3(n);               /* factor 3ⁿ */

    /* multiply by every prime p > 3 with p-1 | n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d, prime candidate p = d + 1 */
            idx_t p = d + 1;
            if (p > 3 && is_prime(p))
                denom *= (seq_t)p;

            /* matched divisor n/d */
            idx_t d2 = n / d;
            if (d2 != d) {
                p = d2 + 1;
                if (p > 3 && is_prime(p))
                    denom *= (seq_t)p;
            }
        }
    }
    return denom;
}
