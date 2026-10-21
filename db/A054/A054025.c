/*
 * A054025  Sum of divisors of n read modulo (number of divisors of n).
 * https://oeis.org/A054025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) mod tau(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses an O(sqrt(n)) divisor enumeration with 64‑bit accumulation
   to avoid overflow, then casts the result to seq_t. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Step 1: count the number of divisors (tau). */
    uint64_t tau = 0;
    for (idx_t i = 1; (unsigned __int128)i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            tau += (i == j) ? 1U : 2U;
        }
    }

    /* Pulse: tau is always ≥1 for n≥1. */
    if (tau == 0) return (seq_t)0;

    /* Step 2: compute sigma(n) modulo tau. */
    uint64_t sigma_mod = 0;
    for (idx_t i = 1; (unsigned __int128)i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            if (i == j) {                 /* perfect square */
                sigma_mod += (uint64_t)i % tau;
                if (sigma_mod >= tau) sigma_mod -= tau;
            } else {                       /* distinct pair of divisors */
                sigma_mod += (uint64_t)i % tau;
                if (sigma_mod >= tau) sigma_mod -= tau;

                sigma_mod += (uint64_t)j % tau;
                if (sigma_mod >= tau) sigma_mod -= tau;
            }
        }
    }

    return (seq_t)sigma_mod;
}
