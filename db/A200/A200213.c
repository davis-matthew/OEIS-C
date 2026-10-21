/*
 * A200213  Ordered factorizations of n with 2 distinct parts, both > 1.
 * https://oeis.org/A200213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(n) - 2 - ([n is a perfect square] && sqrt(n)>1 ? 1 : 0)
 *            where d(n) is the number of positive divisors of n
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                     /* a(1) = 0 */

    /* Compute divisor count d(n) by factoring n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t d = 1;
    for (size_t i = 0; i < cnt; ++i)
        d *= (exps[i] + 1);

    /* Detect perfect squares (sqrt(n) > 1). */
    seq_t sq = floor_sqrt((seq_t)n);
    bool is_sq = ((sq * sq) == (seq_t)n) && (sq > 1);

    /* a(n) = d - 2 - (is_sq ? 1 : 0). */
    seq_t result = (seq_t)(d - 2) - (seq_t)(is_sq ? 1 : 0);
    return result;
}
