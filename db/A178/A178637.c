/*
 * A178637  a(n) = sum of divisors d of n such that d is not equal to p^k
 *          where p = prime, k >=1.
 * https://oeis.org/A178637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000203(n) - A023889(n)
 * Range:     u8 n <= 16, u16 n <= 1467, u32 n <= 9855, u64 n <= 9855,
 *            u128 n <= 9855, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined below the offset */

    uint64_t m = (uint64_t)n;
    seq_t sigma   = 1;
    seq_t sum_pp  = 0;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m      /= p;
                ++e;
            }

            seq_t sp = (seq_t)p;

            /* sigma contribution: (p^(e+1)-1)/(p-1) */
            seq_t pow_e1 = 1;
            for (uint64_t i = 0; i < e + 1; ++i) {
                pow_e1 *= sp;
            }
            seq_t term_sigma = (pow_e1 - 1) / (sp - 1);
            sigma *= term_sigma;

            /* sum of prime‑power divisors: p + p^2 + … + p^e */
            seq_t pow_p = sp;
            for (uint64_t i = 0; i < e; ++i) {
                sum_pp += pow_p;
                pow_p  *= sp;
            }
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        uint64_t p = m;
        uint64_t e = 1;
        seq_t sp = (seq_t)p;

        seq_t pow_e1 = 1;
        for (uint64_t i = 0; i < e + 1; ++i)
            pow_e1 *= sp;
        seq_t term_sigma = (pow_e1 - 1) / (sp - 1);
        sigma *= term_sigma;

        seq_t pow_p = sp;
        for (uint64_t i = 0; i < e; ++i) {
            sum_pp += pow_p;
            pow_p  *= sp;
        }
    }

    return sigma - sum_pp;
}
