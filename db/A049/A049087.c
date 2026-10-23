/*
 * A049087  Ceiling of mean distance between successive distinct prime
 *          divisors of n.
 * https://oeis.org/A049087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil( Σ_{i=1}^{k-1} (p_{i+1}-p_i) / (k-1) ) where p_i are
 *            the distinct prime divisors of n (k=#distinct primes). If n has
 *            0 or 1 distinct prime divisor, a(n)=0
 * Range:     u8 n <= 525, u16 n <= 9364, u32 n <= 9364, u64 n <= 9364,
 *            u128 n <= 9364, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    uint64_t nn = (uint64_t)n;
    uint64_t pr[6];
    uint64_t ex[6];
    size_t cnt = 0;

    factor_u64(nn, pr, ex, &cnt);

    if (cnt <= 1)
        return 0;

    uint64_t sum = 0;
    for (size_t i = 0; i + 1 < cnt; ++i)
        sum += pr[i+1] - pr[i];

    size_t denom = cnt - 1;
    seq_t result = (seq_t)(sum + (denom - 1)) / (seq_t)denom;
    return result;
}
