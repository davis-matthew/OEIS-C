/*
 * A152989  Sum of proper divisors minus the number of proper divisors of
 *          triangular number A000217(n).
 * https://oeis.org/A152989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|T_n, d<T_n} d - (#proper divisors of T_n) where
 *            T_n = n*(n+1)/2. Equivalently a(n) = sigma(T_n) - T_n - tau(T_n)
 *            + 1
 * Range:     u8 n <= 19, u16 n <= 223, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for the OEIS sequence A152989. */
seq_t a(idx_t n)
{
    /* Compute the n-th triangular number */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* For factoring we work with a 64-bit integer (t fits in 64-bit for n up to ~10^4). */
    uint64_t t64 = (uint64_t)t;
    uint64_t temp = t64;

    /* sigma(t) and tau(t) product accumulators */
    seq_t sigma = 1;
    seq_t tau   = 1;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }

            /* Compute sigma_factor = 1 + p + p^2 + ... + p^exp */
            seq_t sigma_factor = 1;
            seq_t term = 1;
            for (uint64_t i = 1; i <= exp; ++i) {
                term *= p;
                sigma_factor += term;
            }
            sigma *= sigma_factor;
            tau *= (exp + 1);
        }
    }

    /* Remaining prime factor (if any) */
    if (temp > 1) {
        seq_t sigma_factor = 1 + (seq_t)temp;
        sigma *= sigma_factor;
        tau *= 2;
    }

    /* a(n) = sigma(t) - t - tau(t) + 1 */
    seq_t result = sigma - t;
    result -= tau;
    result += 1;
    return result;
}
