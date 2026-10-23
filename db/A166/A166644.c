/*
 * A166644  Totally multiplicative sequence with a(p) = 4*(p+1) for prime p.
 * https://oeis.org/A166644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e||n} (4*(p+1))^e
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t base = (seq_t)4 * ((seq_t)primes[i] + (seq_t)1);
        seq_t pow = (seq_t)1;
        for (uint64_t e = 0; e < exps[i]; ++e)
            pow *= base;
        result *= pow;
    }
    return result;
}
