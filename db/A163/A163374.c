/*
 * A163374  a(n) = tau(tau(phi(n))).
 * https://oeis.org/A163374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(tau(phi(n)))
 * Range:     u8 n <= 283, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 102 terms through n = 830 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

/* Count divisors of a 64‑bit integer */
static uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t tau = 1;
    uint64_t exp;

    /* factor 2 */
    exp = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        ++exp;
    }
    if (exp) tau *= (exp + 1);

    /* factor odd primes */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }

    /* remaining prime factor */
    if (n > 1) tau *= 2;

    return tau;
}

seq_t a(idx_t n)
{
    /* Euler's totient of n */
    seq_t phi_n = phi(n);

    /* Count divisors of phi(n) */
    uint64_t phi_val = (uint64_t)phi_n;
    seq_t tau_phi = divisor_count_u64(phi_val);

    /* Count divisors of tau(phi(n)) */
    uint64_t tau_phi_val = (uint64_t)tau_phi;
    seq_t result = divisor_count_u64(tau_phi_val);

    return result;
}
