/*
 * A073802  Number of common divisors of n and sigma(n).
 * https://oeis.org/A073802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(gcd(n, σ(n)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9993 (2026-11-01)
 * Helpers:   sigma_u64_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sigma_u64_to_u64.h"

/* Divisor count τ(n) for a uint64_t.   */
static uint64_t tau_uint64(uint64_t n) {
    if (n == 0) return 0;
    uint64_t count = 1;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            count *= (exp + 1);
        }
    }
    if (temp > 1) {
        count *= 2;     /* last prime factor with exponent 1 */
    }
    return count;
}

/* Euclidean algorithm for uint64_t gcd. */
static uint64_t gcd_uint64(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for n=0, return 0 */

    uint64_t n64 = (uint64_t)n;
    uint64_t sigma = sigma_u64_to_u64(n64);
    uint64_t g = gcd_uint64(n64, sigma);
    uint64_t tau = tau_uint64(g);
    return (seq_t)tau;
}
