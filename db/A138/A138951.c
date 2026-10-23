/*
 * A138951  Expansion of eta(q^2)^7 * eta(q^3)^2 * eta(q^12) / (eta(q)^2 *
 *          eta(q^4)^3 * eta(q^6)^3) in powers of q.
 * https://oeis.org/A138951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2 * b(n) where b is multiplicative: * b(2^e) = -1 (e > 0)
 *            * b(3^e) = -1 + 2*(-1)^e * b(p^e) = e+1 for p ≡ 1 or 5 (mod 12)
 *            * b(p^e) = (1+(-1)^e)/2 for p ≡ 7 or 11 (mod 12) * The case n =
 *            0 returns 1 (constant term of the GF)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   prime_power_b
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prime_power_b.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* constant term */
        return (seq_t)1;

    seq_t res = 1;
    idx_t tmp = (idx_t)n;

    /* handle factor 2 */
    idx_t e = 0;
    while ((tmp & 1ULL) == 0) {          /* tmp % 2 == 0 */
        tmp >>= 1;
        e++;
    }
    if (e > 0)
        res *= prime_power_b(2ULL, e);

    /* handle odd factors */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            if (e > 0)
                res *= prime_power_b(p, e);
        }
    }
    /* remaining prime > sqrt(original n), if any */
    if (tmp > 1) {
        res *= prime_power_b(tmp, 1);
    }

    /* final value a(n) = 2 * b(n) */
    return res * 2;
}
