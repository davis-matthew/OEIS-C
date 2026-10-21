/*
 * A239051  Expansion of (f(-q^2, -q^3)^5 - 3 * q * f(-q, -q^4)^5) / f(-q)^3
 *          in powers of q where f() is a Ramanujan theta function.
 * https://oeis.org/A239051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; for n>0, a(n) = 10 * (d_{2mod5}(n) - d_{3mod5}(n)),
 *            where d_{r}(n) is the number of divisors of n that are ≡ r (mod
 *            5)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;

    idx_t d2 = 0, d3 = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            if (d1 % 5 == 2) ++d2;
            else if (d1 % 5 == 3) ++d3;

            idx_t d2div = n / d1;
            if (d2div != d1) {
                if (d2div % 5 == 2) ++d2;
                else if (d2div % 5 == 3) ++d3;
            }
        }
    }
    return (seq_t)10 * ((seq_t)d2 - (seq_t)d3);
}
