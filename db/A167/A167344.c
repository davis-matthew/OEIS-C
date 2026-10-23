/*
 * A167344  Totally multiplicative sequence with a(p) = (p-1)*(p+1) = p^2-1
 *          for prime p.
 * https://oeis.org/A167344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A167344: Totally multiplicative sequence with a(p) = (p-1)*(p+1) = p^2-1 for prime p.
    Formula:  a(n) = ∏_{p^e || n} (p^2-1)^e   (product over prime powers dividing n)*/

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)                    /* a(1) = 1 */
        return (seq_t)1;

    /* Temporary arrays for prime factors and their exponents.
       64 elements are more than enough for any 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factor the index n. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the product (p^2-1)^e for every prime divisor. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t base = p * p - (seq_t)1;            /* p^2-1 */
        seq_t power = big_pow(base, (seq_t)exps[i]);  /* (p^2-1)^e */
        result *= power;                           /* multiply into result */
    }

    return result;
}
