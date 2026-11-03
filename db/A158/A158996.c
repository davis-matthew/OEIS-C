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
 * Validated: standard, pass, 34 terms through n = 245 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
