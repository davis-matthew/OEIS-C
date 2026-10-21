/*
 * A063749  a(n) = floor(d(n)*(n+1)/2) - sigma(n), where d = A000005, sigma =
 *          A000203.
 * https://oeis.org/A063749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(d(n)*(n+1)/2) - sigma(n)
 * Range:     u8 n <= 83, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* d(n) = A000005 (number of divisors), sigma(n) = A000203 (sum of divisors) */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    uint64_t m = n;              /* working copy for factorisation */
    seq_t tau   = 1;             /* divisor count d(n) */
    seq_t sigma = 1;             /* sum of divisors sigma(n) */

    /* Trial division up to sqrt(m) */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            unsigned int exp = 0;
            while (m % p == 0) {
                ++exp;
                m /= p;
            }

            /* Update τ: multiply by (exp + 1) */
            tau *= (seq_t)(exp + 1);

            /* Compute the divisor‑sum factor for p^exp:
               sum = 1 + p + p^2 + … + p^exp            */
            seq_t term = 1;           /* running sum */
            seq_t mul  = 1;           /* current power of p */
            for (unsigned int i = 0; i < exp; ++i) {
                mul *= (seq_t)p;
                term += mul;
            }
            sigma *= term;
        }
    }

    /* Remaining prime factor (exponent 1) */
    if (m > 1) {
        tau   *= 2;               /* exponent = 1 → factor (1+1) */
        sigma *= (seq_t)(1 + m);
    }

    /* Compute floor( τ * (n+1) / 2 ) using a wider type for safety */
    seq_t floor_val = (seq_t)(((uint64_t)tau * ((uint64_t)n + 1)) / 2);

    /* Final value a(n) */
    return floor_val - sigma;
}
