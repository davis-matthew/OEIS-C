/*
 * A158996  Denominator of Bernoulli(n, -1/10).
 * https://oeis.org/A158996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 5·10^(n-1) for odd n; * a(0) = 1; * for even n > 0: * let
 *            P(n) = ∏{ p prime | p-1 | n } p, * let d = 10 if n mod 4 == 0,
 *            else 2, * a(n) = (P(n)/d) · 10^n
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_mul.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* odd n */
    if (n & 1U) {
        seq_t pow10n = pow10(n - 1U);
        return seq_mul(5U, pow10n);
    }

    /* even n */
    seq_t prod = 1U;

    /* Enumerate all divisors d of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            if (is_prime(d1 + 1U))
                prod = seq_mul(prod, d1 + 1U);

            if (d1 != d2 && is_prime(d2 + 1U))
                prod = seq_mul(prod, d2 + 1U);
        }
    }

    /* Divide by 10 or 2 */
    idx_t divisor = (n % 4U == 0U) ? 10U : 2U;
    prod = prod / divisor;

    seq_t pow10n = pow10(n);
    return seq_mul(prod, pow10n);
}
