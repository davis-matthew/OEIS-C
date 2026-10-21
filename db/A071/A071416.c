/*
 * A071416  a(n) = gcd(n, binomial(2*n, n)).
 * https://oeis.org/A071416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = gcd(n, binomial(2*n, n))
 * Range:     u8 n <= 265, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static unsigned long long legendre(long long m, unsigned long long p) {
    unsigned long long exp = 0;
    __uint128_t p_pow = p;
    __uint128_t limit = (m <= 0) ? 0 : (unsigned long long)m;
    while (p_pow <= limit) {
        exp += (unsigned long long)(limit / p_pow);
        p_pow *= p;
    }
    return exp;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but guard against n==0 */

    /* Convert index to 64-bit unsigned for arithmetic */
    uint64_t nn = (uint64_t)n;

    /* Factor n: primes and exponents */
    uint64_t primes[65];
    uint64_t exps[65];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        unsigned long long p = primes[i];
        unsigned long long e_n     = exps[i];

        /* exponent of p in (2n)! and in n! */
        unsigned long long e_2n = legendre(2 * nn, p);
        unsigned long long e_np = legendre(nn, p);

        unsigned long long e_bin = e_2n - 2 * e_np;
        unsigned long long e_gcd = (e_n < e_bin) ? e_n : e_bin;

        /* multiply result by p^e_gcd; product never exceeds n */
        for (unsigned long long k = 0; k < e_gcd; ++k) {
            result = result * (seq_t)p;
        }
    }
    return result;
}
