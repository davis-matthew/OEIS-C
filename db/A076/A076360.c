/*
 * A076360  a(n) = d(sigma(n)) - sigma(d(n)), where d(n) is the number of
 *          divisors of n and sigma(n) is their sum.
 * https://oeis.org/A076360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = d(sigma(n)) - sigma(d(n))
 * Range:     i8 n <= 8399, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9400 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the number of divisors (tau) and the sum of divisors (sigma)
   of a positive integer n using trial division.  Results are placed
   into *tau_out and *sigma_out.  The function works for 64‑bit inputs
   without overflow.  */
static void factor_and_compute(uint64_t n, uint64_t *tau_out, uint64_t *sigma_out)
{
    uint64_t remaining = n;
    uint64_t tau = 1;
    unsigned __int128 sigma = 1;

    /* factor 2 */
    if ((remaining & 1ULL) == 0) {
        int e = 0;
        while ((remaining & 1ULL) == 0) {
            remaining >>= 1;
            ++e;
        }
        tau *= (uint64_t)(e + 1);

        unsigned __int128 powp = 1;
        for (int i = 0; i < e + 1; ++i) powp *= 2ULL;
        unsigned __int128 term = (powp - 1) / (2ULL - 1);
        sigma *= term;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= remaining / p; p += 2) {
        if ((remaining % p) != 0) continue;
        int e = 0;
        while (remaining % p == 0) {
            remaining /= p;
            ++e;
        }
        tau *= (uint64_t)(e + 1);

        unsigned __int128 powp = 1;
        for (int i = 0; i < e + 1; ++i) powp *= p;
        unsigned __int128 term = (powp - 1) / (p - 1);
        sigma *= term;
    }

    /* if remaining is a prime greater than sqrt(n) */
    if (remaining > 1) {
        tau *= 2;
        sigma *= (remaining + 1);
    }

    *tau_out = tau;
    *sigma_out = (uint64_t)sigma;
}

/* Compute sigma for a small integer (up to about 2^20) by simple enumeration.
   The parameter x is the integer whose divisor sum is required.  The result
   is stored in *sigma_out.  */
static void sigma_of_small(uint64_t x, uint64_t *sigma_out)
{
    uint64_t sigma = 1;
    for (uint64_t d = 2; d <= x / d; ++d) {
        if (x % d == 0) {
            uint64_t d2 = x / d;
            sigma += d + d2;
        }
    }
    if (x > 1) sigma += 1;
    *sigma_out = sigma;
}

/* Main sequence function.  It may be called with any order of indices. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1, return 0 */

    uint64_t nn = (uint64_t)n;
    uint64_t tau_n, sigma_n;
    factor_and_compute(nn, &tau_n, &sigma_n);

    uint64_t tau_sigma, dummy;
    factor_and_compute(sigma_n, &tau_sigma, &dummy);

    uint64_t sigma_tau;
    /* tau_n is small, we can compute its sigma by factoring directly. */
    factor_and_compute(tau_n, &dummy, &sigma_tau);

    /* The difference may be negative, use signed integer arithmetic. */
    int64_t result = (int64_t)tau_sigma - (int64_t)sigma_tau;
    return (seq_t)result;
}
