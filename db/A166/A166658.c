/*
 * A166658  Totally multiplicative sequence with a(p) = 9p-1 for prime p.
 * https://oeis.org/A166658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e dividing n of (9p-1)^e,
 *            with a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    uint64_t m = (uint64_t)n;
    /* arrays big enough for the prime factorisation of any 64‑bit integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base = (seq_t)9 * (seq_t)primes[i] - (seq_t)1;
        seq_t power = (seq_t)1;
        for (int e = 0; e < (int)exps[i]; ++e) power *= base;
        result *= power;
    }
    return result;
}
