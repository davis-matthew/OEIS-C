/*
 * A023140  Number of cycles of function f(x) = 8x mod n.
 * https://oeis.org/A023140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|m} phi(d)/ord(8,d) where m is n with all factors
 *            of 2 removed
 * Range:     u8 n <= 1364, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   modPow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/modPow_uint64.h"

static uint64_t phi_uint64(uint64_t n) {
    uint64_t result = n;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

static uint64_t ord8(uint64_t d) {
    if (d == 1) return 1;
    uint64_t phi = phi_uint64(d);
    for (uint64_t k = 1; k <= phi; ++k) {
        if (phi % k == 0) {
            if (modPow_uint64(8ULL, k, d) == 1ULL) return k;
        }
    }
    return phi; /* fallback, should never happen */
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    while ((m & 1ULL) == 0) m >>= 1;          /* remove all factors of 2 */

    seq_t sum = 0;
    for (uint64_t d = 1; d <= m; ++d) {
        if (m % d != 0) continue;             /* d must divide m */
        uint64_t phi_d = phi_uint64(d);
        uint64_t ord = ord8(d);
        sum += (seq_t)(phi_d / ord);           /* phi_d and ord fit in 64 bits */
    }
    return sum;
}
