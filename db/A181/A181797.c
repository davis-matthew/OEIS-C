/*
 * A181797  a(n) = n multiplied by the sum of its squarefree divisors
 *          (A048250(n)).
 * https://oeis.org/A181797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * A048250(n) = product_{p^e||n} (p+1) * p^e
 * Range:     u8 n <= 13, u16 n <= 185, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];
        seq_t pow = (seq_t)1;
        for (uint64_t j = 0; j < e; ++j) pow *= p;

        seq_t term = (seq_t)(primes[i] + 1) * pow;
        result *= term;
    }
    return result;
}
