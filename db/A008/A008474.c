/*
 * A008474  If n = Product (p_j^k_j) then a(n) = Sum (p_j + k_j).
 * https://oeis.org/A008474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p^k || n} (p + k) for n ≥ 1, a(1) = 0
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses factor_u64 to perform the prime factorisation. */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    /* Largest distinct prime factor count for a 64‑bit integer is < 64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)primes[i] + (seq_t)exps[i];

    return sum;
}
