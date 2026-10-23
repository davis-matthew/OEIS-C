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
 * Helpers:   prime_power_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/prime_power_value.h"

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
