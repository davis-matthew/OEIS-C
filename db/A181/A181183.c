/*
 * A181183  a(n) = sigma(tau(n)) (mod 2).
 * https://oeis.org/A181183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(tau(n)) (mod 2)
 * Range:     u8 n <= 9485, u16 n <= 9485, u32 n <= 9485, u64 n <= 9485,
 *            u128 n <= 9485, big any n
 * Helpers:   factor_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   sigma(tau(n)) is odd iff tau(n) is a square or twice a square.      */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Compute the divisor count τ(n). */
    uint64_t nn = (uint64_t)n;
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(nn, primes, exps, &cnt);

    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (seq_t)(exps[i] + 1);

    /* Determine parity of sigma(tau).  sigma(m) is odd ⇔ m is a square or twice a square. */
    seq_t r = floor_sqrt(tau);
    if (r * r == tau)
        return (seq_t)1;

    seq_t half = tau / 2;
    seq_t s = floor_sqrt(half);
    if (s * s * 2 == tau)
        return (seq_t)1;

    return (seq_t)0;
}
