/*
 * A166651  Totally multiplicative sequence with a(p) = 2p-1 for prime p.
 * https://oeis.org/A166651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (2p‑1)^e Totally multiplicative sequence with
 *            a(p) = 2p-1 for prime p
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* factor_u64 returns the distinct prime factors in primes[] and their
       corresponding exponents in exps[], with the count in cnt. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base = (seq_t)2 * (seq_t)primes[i] - (seq_t)1;
        for (uint64_t e = 0; e < exps[i]; ++e)
            result *= base;
    }
    return result;
}
