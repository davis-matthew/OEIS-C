/*
 * A133988  Expansion of phi(x) / chi(x^3) in powers of x where phi(), chi()
 *          are Ramanujan theta functions.
 * https://oeis.org/A133988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(8 n + 1) where b() is multiplicative and • b(p^(2 e)) =
 *            -2 · (‑1)^e if p = 3 • b(p^(2 e)) = (‑1)^e if p ≡ 3,5 (mod 8) •
 *            b(p^(2 e)) = +1 if p ≡ 1,7 (mod 8) • b(p^(2 e - 1)) = 0 for any
 *            prime p (odd exponent) • b(2^e) = 0 e > 0 (even powers of 2
 *            never appear in 8n+1)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   b_prime_power_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/b_prime_power_value.h"

seq_t a(idx_t n)
{
    idx_t m = 8 * n + 1;                         /* 8n+1 is always odd */
    seq_t result = 1;

    if (m == 1) {
        return 1;
    }

    /* iterate over odd divisors starting at 3 */
    idx_t d = 3;
    while (d * d <= m) {
        if (m % d == 0) {
            idx_t exp = 0;
            while (m % d == 0) {
                m /= d;
                ++exp;
            }
            seq_t val = b_prime_power_value(d, exp);
            if (!val) return 0;                /* product becomes zero */
            result *= val;
        }
        d += 2;
    }

    if (m > 1) {                                 /* residue prime factor */
        seq_t val = b_prime_power_value(m, 1);    /* exponent 1 is odd → 0 */
        if (!val) return 0;
        result *= val;
    }

    return result;
}
