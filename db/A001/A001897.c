/*
 * A001897  Denominators of cosecant numbers: -2*(2^(2*n-1)-1)*Bernoulli(2*n).
 * https://oeis.org/A001897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(0) = 1; a(n) = A002445(n)/2 = Product_{p prime, p > 2, (p-1) |
 *            2n} p for n >= 1 (von Staudt-Clausen)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;

    uint64_t m = 2 * (uint64_t)n;
    seq_t result = 1;

    for (uint64_t d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            uint64_t p1 = d + 1;
            if (p1 > 2 && is_prime_u64(p1)) {
                result = result * (seq_t)p1;
            }
            uint64_t d2 = m / d;
            uint64_t p2 = d2 + 1;
            if (d != d2 && p2 > 2 && is_prime_u64(p2)) {
                result = result * (seq_t)p2;
            }
        }
    }

    return result;
}
