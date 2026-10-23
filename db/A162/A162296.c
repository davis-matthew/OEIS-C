/*
 * A162296  Sum of divisors of n that have a square factor.
 * https://oeis.org/A162296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n) - product_{p|n} (1 + p) where sigma(n) is the
 *            sum of all divisors of n. This computes the sum of divisors that
 *            contain a square factor
 * Range:     u8 n <= 107, u16 n <= 9987, u32 n <= 9987, u64 n <= 9987,
 *            u128 n <= 9987, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    __uint128_t sigma = 1;
    __uint128_t sf = 1;       /* sum of square‑free divisors */
    uint64_t t = nn;

    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            __uint128_t term = 1;   /* 1 + p + p^2 + ... + p^e */
            __uint128_t pe = 1;     /* p^e */
            while (t % p == 0) {
                t /= p;
                pe *= p;
                term += pe;
            }
            sigma *= term;
            sf *= (1 + (__uint128_t)p);
        }
    }
    if (t > 1) {                /* remaining prime factor */
        sigma *= (1 + (__uint128_t)t);
        sf *= (1 + (__uint128_t)t);
    }

    return (seq_t)(sigma - sf);
}
