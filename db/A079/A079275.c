/*
 * A079275  Number of divisors of n that are semiprimes with distinct factors.
 * https://oeis.org/A079275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ω(n) * (ω(n) - 1) / 2 * (where ω(n) is the number of
 *            distinct prime factors of n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the result is 0 */
    if (n < 2)
        return 0;

    /* Factor n to count distinct prime factors */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    /* factor_u64 is defined in factor_u64.h as a static helper */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* ω(n) fits in seq_t here (cnt is small) */
    seq_t k = (seq_t)cnt;
    return k * (k - 1) / 2;
}
