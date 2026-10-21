/*
 * A238949  Degree of divisor lattice D(n).
 * https://oeis.org/A238949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p^e||n} (1 + [e≥2]) where p^e||n means p^e divides n
 *            and p^(e+1) does not. Thus a(n) counts each distinct prime
 *            factor once, plus an additional 1 for every prime with exponent
 *            at least 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    seq_t result = 0;
    uint64_t temp = nn;

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            size_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            result += 1;          /* distinct prime */
            if (e >= 2) result += 1; /* extra for exponent ≥ 2 */
        }
    }
    if (temp > 1) {
        /* temp is a prime factor > sqrt(original n), exponent is 1 */
        result += 1;
    }
    return result;
}
