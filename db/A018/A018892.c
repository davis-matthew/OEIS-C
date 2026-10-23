/*
 * A018892  Number of ways to write 1/n as a sum of exactly 2 unit fractions.
 * https://oeis.org/A018892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (tau(n^2)+1)/2 = number of divisors of n^2 <= n
 * Range:     u8 n <= 9239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factor n into primes and exponents. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t) n, primes, exps, &cnt);

    /* Compute product of (2*exp + 1). */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        prod *= (seq_t)(2 * exps[i] + 1);
    }

    /* Return (product + 1)/2. */
    return (prod + 1) / 2;
}
