/*
 * A212181  Largest odd divisor of tau(n): a(n) = A000265(A000005(n)).
 * https://oeis.org/A212181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = largest odd divisor of tau(n) = A000265(tau(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                   /* undefined for n < 1, guard */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factorize n */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute tau(n) = product of (exponent + 1) */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        tau = tau * ((seq_t)exps[i] + 1);
    }

    /* remove all factors of 2 to obtain largest odd divisor */
    while ((tau & 1) == 0)
        tau >>= 1;

    return tau;
}
