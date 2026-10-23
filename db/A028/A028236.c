/*
 * A028236  If n = Product (p_j^k_j), a(n) = numerator of Sum 1/p_j^k_j.
 * https://oeis.org/A028236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of Σ 1/p^k over the distinct prime powers p^k
 *            in the factorization of n; a(1) = 1. The numerator can be
 *            computed as Σ (n / p^k), i.e., sum of n divided by each prime
 *            power
 * Range:     u8 n <= 317, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* handle the special case n == 1 */
    if (n == 1u) return (seq_t)1;

    /* n fits in idx_t (uint64_t).  Convert to seq_t for arithmetic. */
    seq_t n_val = (seq_t)n;

    /* arrays large enough to hold all distinct prime factors of a 64‑bit number */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* compute p^k */
        seq_t power = (seq_t)1;
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        for (uint64_t j = 0; j < e; ++j)
            power *= (seq_t)p;

        /* add n / p^k to the sum */
        sum += n_val / power;
    }

    return sum;
}
