/*
 * A068903  Binomial(tau(n),omega(n)), where tau(n) is the number of divisors
 *          of n (A000005) and omega the number of distinct prime factors
 *          (A001221).
 * https://oeis.org/A068903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( tau(n), omega(n) ) where tau(n) is the number
 *            of divisors of n and omega(n) is the number of distinct prime
 *            factors of n
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Safety check: n must be >= 1 as per the sequence definition. */
    if (n == 0)
        return 0;

    /* Factor n: primes[], exps[] will hold the prime factors and their
       exponents, and cnt will contain the number of distinct primes (omega). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute tau(n) = product_{i}(exps[i] + 1). */
    idx_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (exps[i] + 1);

    idx_t omega = (idx_t)cnt;

    if (omega > tau)   /* Should never happen for n >= 1 */
        return 0;

    return binomial(tau, omega);
}
