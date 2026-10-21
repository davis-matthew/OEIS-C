/*
 * A055205  Number of nonsquare divisors of n^2.
 * https://oeis.org/A055205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = d(n^2) - d(n) where d(m) is the number of positive
 *            divisors of m
 * Range:     u8 n <= 2519, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include <stddef.h>

seq_t a(idx_t n) {
    uint64_t x = (uint64_t)n;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(x, primes, exps, &cnt);

    seq_t dn = 1;   /* d(n)      */
    seq_t dn2 = 1;  /* d(n^2)    */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = exps[i];
        dn  *= (e + 1);
        dn2 *= (2 * e + 1);
    }
    return dn2 - dn;
}
