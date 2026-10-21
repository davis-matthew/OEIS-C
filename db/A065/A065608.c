/*
 * A065608  Sum of divisors of n minus the number of divisors of n.
 * https://oeis.org/A065608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) - d(n) where sigma(n)=sum of divisors,
 *            d(n)=number of divisors
 * Range:     u8 n <= 107, u16 n <= 9709, u32 n <= 9709, u64 n <= 9709,
 *            u128 n <= 9709, big any n
 * Helpers:   seq_pow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow_uint64.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    seq_t sigma = 1;      /* product of (p^(e+1)-1)/(p-1) for all prime factors */
    seq_t tau   = 1;      /* product of (e+1)  — the divisor count */

    /* factor 2 separately */
    if (m % 2 == 0) {
        uint64_t e = 0;
        while (m % 2 == 0) {
            m /= 2;
            e++;
        }
        seq_t poly = 1;                     /* 1 + 2 + 2^2 + ... + 2^e */
        seq_t term = 1;
        for (uint64_t i = 0; i < e; i++) {
            term *= 2;
            poly += term;
        }
        sigma *= poly;
        tau   *= (e + 1);
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            seq_t poly = 1;
            seq_t term = 1;
            seq_t base = (seq_t)p;
            for (uint64_t i = 0; i < e; i++) {
                term *= base;
                poly += term;
            }
            sigma *= poly;
            tau   *= (e + 1);
        }
    }

    /* remainder is a prime > sqrt(original n) */
    if (m > 1) {
        seq_t term = (seq_t)m;
        sigma *= (1 + term);
        tau   *= 2;
    }

    return sigma - tau;
}
