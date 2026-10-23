/*
 * A167327  Totally multiplicative sequence with a(p) = 8*(p+3) for prime p.
 * https://oeis.org/A167327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 95, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, factor_u64, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = ∏_{p^e‖n} (8*(p + 3))^e
 * (totally multiplicative with a(p) = 8*(p+3) for prime p)
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* offset is 1, but guard against 0 */
        return 0;

    uint64_t nn = (uint64_t)n;

    /* Allocate a small buffer for prime factors (should be enough for 64-bit n) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;

    factor_u64(nn, primes, exps, &count);

    seq_t result = 1;

    for (size_t i = 0; i < count; ++i)
    {
        /* base = 8 * (p + 3) */
        seq_t base = (seq_t)(8 * (primes[i] + 3));

        /* exponent = e */
        seq_t exp  = (seq_t)exps[i];

        seq_t term = big_pow(base, exp);

        result = seq_mul(result, term);
    }

    return result;
}
