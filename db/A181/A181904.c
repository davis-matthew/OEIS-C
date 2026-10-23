/*
 * A181904  a(n) = 2*(4^n - 1) / A027760(n).
 * https://oeis.org/A181904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^n - 1) / Π_{p odd prime, (p-1) | n} p (The product is
 *            taken over odd primes p such that p-1 divides n.) The value is
 *            computed by enumerating the divisors of n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/is_prime.h"

static seq_t _clausen_denominator(idx_t n)
{
    /* r(n) = product of odd primes p with p-1 | n */
    seq_t r = 1;

    /* Check divisor n itself: p = n+1 */
    if (is_prime((idx_t)(n + 1)) && !( (n + 1) == 2 )) {
        r *= (seq_t)(n + 1);
    }

    /* Enumerate proper divisors d (>=2) and their pairs */
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            /* prime p = d+1 */
            if (is_prime((idx_t)(d1 + 1)) && !( (d1 + 1) == 2 )) {
                r *= (seq_t)(d1 + 1);
            }
            /* second divisor only if distinct */
            if (d2 != d1) {
                if (is_prime((idx_t)(d2 + 1)) && !( (d2 + 1) == 2 )) {
                    r *= (seq_t)(d2 + 1);
                }
            }
        }
    }

    return r;
}

seq_t a(idx_t n)
{
    /* Compute denominator r(n) */
    seq_t denom = _clausen_denominator(n);

    /* numerator: 4^n - 1 */
    seq_t num = pow_uint(4, n);
    num -= 1;

    /* result */
    return num / denom;
}
