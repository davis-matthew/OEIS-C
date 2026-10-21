/*
 * A082068  Smallest common prime-divisor of n and sigma(n) = A000203(n);
 *          a(n)=1 if no common prime-divisor exists.
 * https://oeis.org/A082068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9726, u16 n <= 9726, u32 n <= 9726, u64 n <= 9726,
 *            u128 n <= 9726, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest common prime divisor of n and sigma(n) (1 if none) */
#include <stdint.h>
#include "../utils/gcd_uint64.h"

static uint64_t smallest_prime_factor_uint64(uint64_t x) {
    if (x <= 1) return x;          /* will only be called with x >= 2 */
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) return p;
    }
    return x;                       /* x is prime */
}

/* compute σ(n) = sum of divisors of n, using trial division */
static uint64_t sigma_uint64(uint64_t n) {
    uint64_t sigma = 1;
    uint64_t remaining = n;
    for (uint64_t p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            /* compute term = (p^{e+1} - 1) / (p - 1) */
            uint64_t powp = 1;         /* p^e */
            while (remaining % p == 0) {
                remaining /= p;
                powp *= p;
            }
            uint64_t numerator   = powp * p - 1; /* p^{e+1} - 1 */
            uint64_t denominator = p - 1;
            uint64_t term = numerator / denominator;
            sigma *= term;
        }
    }
    if (remaining > 1) {
        /* remaining is prime, e = 1 */
        sigma *= (remaining + 1);     /* (p^2-1)/(p-1) = p+1 */
    }
    return sigma;
}

seq_t a(idx_t n) {
    if (n < 1) return 1;            /* offset starts at 1 */
    uint64_t N = (uint64_t)n;
    uint64_t sigma = sigma_uint64(N);
    uint64_t g = gcd_uint64(N, sigma);
    if (g == 1) return (seq_t)1;
    return (seq_t)smallest_prime_factor_uint64(g);
}
