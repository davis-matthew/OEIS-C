/*
 * A011262  In the prime factorization of n, increment odd powers and
 *          decrement even powers (multiplicative and self-inverse).
 * https://oeis.org/A011262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^k|n} p^(k-1) if k even else p^(k+1)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    uint64_t uint_n = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(uint_n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t k = exps[i];
        uint64_t new_exp = (k & 1ULL) ? k + 1 : k - 1;
        seq_t term = (seq_t)1;
        for (uint64_t j = 0; j < new_exp; ++j)
            term *= (seq_t)p;
        result *= term;
    }
    return result;
}
