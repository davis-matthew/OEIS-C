/*
 * A089723  a(1)=1; for n>1, a(n) gives number of ways to write n as n = x^y,
 *          2 <= x, 1 <= y.
 * https://oeis.org/A089723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of perfect divisors of n = d(gcd of its prime
 *            exponents)
 * Range:     u8 n <= 9101, u16 n <= 9101, u32 n <= 9101, u64 n <= 9101,
 *            u128 n <= 9101, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Euclidean GCD for uint64_t. */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Count divisors of a small integer (here g ≤ max exponent of n). */
static seq_t divisor_count_uint64(uint64_t v)
{
    uint64_t dcnt = 1;
    for (uint64_t p = 2; p * p <= v; ++p) {
        if (v % p == 0) {
            uint64_t e = 0;
            while (v % p == 0) {
                v /= p;
                ++e;
            }
            dcnt *= (e + 1);
        }
    }
    if (v > 1) dcnt *= 2;
    return (seq_t)dcnt;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;

    /* factor_u64: factor the 64‑bit integer nn */
    factor_u64(nn, primes, exps, &pcnt);

    uint64_t g = exps[0];
    for (size_t i = 1; i < pcnt; ++i) {
        g = gcd_uint64(g, exps[i]);
    }

    return divisor_count_uint64(g);
}
