/*
 * A101035  Dirichlet inverse of the gcd-sum function (A018804).
 * https://oeis.org/A101035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, sign, mult
 * Formula:   a(n) = ∏_{p^e∥n} f(p^e) where f(p) = 1 - 2p f(p^e) = (p-1)^2 for
 *            e>1 (multiplicative function)
 * Range:     i8 n <= 29, i16 n <= 2309, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9028 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: value of the multiplicative function at a prime power p^e */
static seq_t prime_power_value(idx_t p, unsigned e)
{
    if (e == 1) {
        return (seq_t)(1) - (seq_t)(2) * (seq_t)p;
    } else {
        seq_t pp = (seq_t)p - (seq_t)1;
        return pp * pp;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;
    idx_t temp = n;
    unsigned e;
    /* Factor out 2 separately */
    if ((temp & 1) == 0) {
        e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        result *= prime_power_value(2, e);
    }

    /* Factor odd primes */
    for (idx_t d = 3; d * d <= temp; d += 2) {
        if (temp % d == 0) {
            e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }
            result *= prime_power_value(d, e);
        }
    }

    /* Remaining prime factor (if any) */
    if (temp > 1) {
        result *= prime_power_value(temp, 1);
    }

    return result;
}
