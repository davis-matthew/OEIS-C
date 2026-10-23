/*
 * A134193  a(1) = 1; for n>1, a(n) = the smallest positive integer not
 *          occurring among the exponents in the prime-factorization of n.
 * https://oeis.org/A134193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer not occurring among the
 *            exponents in the prime factorization of n, a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    /* factor_u64 returns primes[] and corresponding exponents[] */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;
    factor_u64((uint64_t)n, primes, exps, &count);

    /* track which exponents appear (exponents are small for 64‑bit n) */
    bool seen[70] = {false};
    for (size_t i = 0; i < count; ++i) {
        uint64_t e = exps[i];
        if (e < sizeof(seen)/sizeof(seen[0]))
            seen[(size_t)e] = true;
    }

    seq_t res = 1;
    while (seen[(size_t)res]) ++res;
    return res;
}
