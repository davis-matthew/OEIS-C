/*
 * A053865  A third-order generalization of the Mobius function of n.
 * https://oeis.org/A053865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0 if any prime factor of n has exponent > 3; a(n) = -1 if
 *            n is a perfect cube of a prime (p^3); a(n) = 1 otherwise
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Normalize */
    uint64_t N = (uint64_t)n;

    /* Special case: 1 has no prime factors */
    if (N == 1) {
        return 1;
    }

    /* Storage for prime factors and exponents.
       64 distinct primes are more than enough for 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    /* Factor N.  factor_u64 is a utility that populates primes, exps, cnt. */
    factor_u64(N, primes, exps, &cnt);

    /* If any exponent > 3, result is 0. */
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > 3) {
            return 0;
        }
    }

    /* If n is a prime cube (p^3), result is -1. */
    if (cnt == 1 && exps[0] == 3) {
        return (seq_t)(-1);
    }

    /* Otherwise result is 1. */
    return 1;
}
