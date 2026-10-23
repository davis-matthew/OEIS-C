/*
 * A175069  a(n) = product of perfect divisors of n / n.
 * https://oeis.org/A175069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (product of perfect divisors of n) / n
 * Range:     u8 n <= 48, u16 n <= 4095, u32 n <= 7668, u64 n <= 7668,
 *            u128 n <= 7668, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Euclidean algorithm for uint64_t, used only inside this file. */
static uint64_t uint64_gcd(uint64_t a, uint64_t b)
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
    /* Handle n = 1 separately (its only divisor is 1, product = 1). */
    if (n == 1U)
        return 1;

    /* Factor n into primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    /* Compute G = gcd of all exponents. */
    uint64_t G = exps[0];
    for (size_t i = 1; i < cnt; ++i)
        G = uint64_gcd(G, exps[i]);

    /* Enumerate all positive divisors of G. */
    idx_t divs[64];
    idx_t dcnt = 0;
    for (idx_t k = 1; k <= G; ++k) {
        if (G % k == 0)
            divs[dcnt++] = k;
    }

    /* Multiply all perfect divisors d_k. */
    seq_t prod = 1;
    for (idx_t di = 0; di < dcnt; ++di) {
        idx_t k = divs[di];
        seq_t d = 1;
        for (size_t j = 0; j < cnt; ++j) {
            idx_t exp = exps[j] / k;
            seq_t p = (seq_t)primes[j];
            for (idx_t e = 0; e < exp; ++e)
                d *= p;
        }
        prod *= d;
    }

    /* Return product / n. */
    return prod / (seq_t)n;
}
