/*
 * A193562  Number of divisors of n^4+1.
 * https://oeis.org/A193562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n^4+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(idx_t n) returning seq_t */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n can be 0, handle separately */
    if (n == 0)
        return (seq_t)1;

    /* compute n^4 + 1 in 64-bit integer */
    uint64_t n64 = (uint64_t)n;
    uint64_t m = n64 * n64;   /* n^2   */
    m *= n64;               /* n^3   */
    m *= n64;               /* n^4   */
    m += (uint64_t)1;        /* n^4+1 */

    /* factor m; primes up to 2^64, so array sizes are ample */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(m, primes, exps, &cnt);

    /* compute divisor count from exponents: ∏(ei+1) */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        result *= (seq_t)(exps[i] + 1);
    }
    return result;
}
