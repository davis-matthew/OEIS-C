/*
 * A063994  a(n) = Product_{primes p dividing n } gcd(p-1, n-1).
 * https://oeis.org/A063994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Π_{p|n} gcd(p‑1, n‑1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Euclidean algorithm for uint64_t values */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* maximum possible distinct prime factors for any 64‑bit integer is 64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t prod = 1;
    uint64_t n_minus1 = n - 1;

    for (size_t i = 0; i < cnt; ++i) {
        /* primes[i] is a prime divisor of n */
        uint64_t g_val = gcd_u64(primes[i] - 1, n_minus1);
        prod *= (seq_t)g_val;
    }
    return prod;
}
