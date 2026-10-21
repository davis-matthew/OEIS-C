/*
 * A235323  Squared sum of the distinct prime factors of n, i.e., sopf(n)^2.
 * https://oeis.org/A235323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of distinct prime factors of n)^2
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* For n = 1 the sum of distinct prime factors is 0, so the result is 0. */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += (seq_t)primes[i];
    }
    return sum * sum;
}
