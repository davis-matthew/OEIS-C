/*
 * A115979  Expansion of (1 - theta_4(q)*theta_4(q^3))/2 in powers of q.
 * https://oeis.org/A115979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) is multiplicative with a(2^e) = -3*(1+(-1)^e)/2 ( e>0 )
 *            a(3^e) = 1 a(p^e) = 1+e if p ≡ 1 (mod 6) a(p^e) = (1+(-1)^e)/2
 *            if p ≡ 5 (mod 6)
 * Range:     i8 n <= 8734, i16 n <= 8734, i32 n <= 8734, i64 n <= 8734,
 *            i128 n <= 8734, big any n
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: value contributed by a prime power p^e. */
#include <stdint.h>
#include "../utils/smallest_prime_factor.h"   /* provides smallest_prime_factor(uint64_t) */

static seq_t prime_power_value(uint64_t p, int exp)
{
    if (p == 2) {
        /* -3 * (1 + (-1)^e) / 2   → 0 for odd e, -3 for even e */
        return (exp & 1) ? (seq_t)0 : (seq_t)-3;
    } else if (p == 3) {
        return (seq_t)1;
    } else if (p % 6 == 1) {
        return (seq_t)(1 + exp);
    } else { /* p % 6 == 5 */
        return (exp & 1) ? (seq_t)0 : (seq_t)1;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);
        if (p == 0) p = m;           /* fallback: m is prime */

        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        seq_t f = prime_power_value(p, exp);
        if (f == (seq_t)0) return (seq_t)0;   /* product becomes zero */
        result *= f;
    }

    return result;
}
