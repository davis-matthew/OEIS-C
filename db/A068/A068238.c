/*
 * A068238  Denominators of arithmetic derivative of 1/n: -A003415(n)/n^2.
 * https://oeis.org/A068238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^2 / gcd(n^2, A003415(n)) where A003415(n) = Σ_{p^k||n}
 *            k * n / p (sum over prime powers dividing n)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute A003415(n) by factoring n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)exps[i] * (seq_t)n / (seq_t)primes[i];
        sum += term;
    }

    /* Denominator = n^2 / gcd(n^2, sum) */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t g = gcd(sum, n_sq);
    seq_t denom = n_sq / g;
    return denom;
}
