/*
 * A129588  Expansion of q^-1 * theta_2(q)^4 in powers of q^2.
 * https://oeis.org/A129588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16 * sigma(2n + 1) (sigma = sum of all positive divisors)
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Work with 64‑bit unsigned integers for factorisation.
       For typical contest inputs (n ≤ 10⁴) 2n+1 fits easily in uint64_t. */
    uint64_t m = 2ULL * (uint64_t)n + 1ULL;

    /* Sum of divisors of 1 is 1. */
    if (m == 1) {
        return (seq_t)16;
    }

    seq_t sigma = 1;
    uint64_t temp = m;

    /* Trial division to factorise m. */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            seq_t term = 1;       /* sum of powers of p : 1 + p + p^2 + ... */
            seq_t p_pow = 1;
            while (temp % p == 0) {
                temp /= p;
                p_pow *= (seq_t)p;
                term += p_pow;
            }
            sigma *= term;
        }
    }

    /* If temp > 1, it is a prime factor > sqrt(m). */
    if (temp > 1) {
        sigma *= ((seq_t)1 + (seq_t)temp);
    }

    return (seq_t)16 * sigma;
}
