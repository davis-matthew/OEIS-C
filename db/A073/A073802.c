/*
 * A073802  Number of common divisors of n and sigma(n).
 * https://oeis.org/A073802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(gcd(n, σ(n)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_uint64, tau_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sigma_uint64.h"
#include "../utils/tau_uint64.h"

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
    uint64_t sigma = sigma_uint64(n64);
    uint64_t g = gcd_uint64(n64, sigma);
    uint64_t tau = tau_uint64(g);
    return (seq_t)tau;
}
