/*
 * A216864  Number of squares that divide the product of divisors of n.
 * https://oeis.org/A216864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 1, otherwise a(n) = ∏(1 + ⌊M·e_i/4⌋) where n =
 *            ∏p_i^e_i and M = ∏(1 + e_i). This computes the number of squares
 *            dividing the product of the divisors of n
 * Range:     u8 n <= 119, u16 n <= 2519, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    /* Factor n into primes and exponents.  The largest number of distinct
       prime factors for a 64‑bit integer is 15, so a buffer of 64 is ample. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute M = ∏(1 + e_i). */
    seq_t M = 1;
    for (size_t i = 0; i < cnt; ++i)
        M *= (seq_t)(exps[i] + 1);

    /* Compute the product of the terms (1 + floor(M·e_i/4)). */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = 1 + (M * (seq_t)exps[i]) / 4;
        result *= term;
    }

    return result;
}
