/*
 * A166668  Totally multiplicative sequence with a(p) = 10p+1 for prime p.
 * https://oeis.org/A166668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e ∥ n} (10p + 1)^e , with a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    /* factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   count = 0;

    factor_u64((uint64_t)n, primes, exps, &count);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < count; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t base = (seq_t)10 * p + (seq_t)1;   /* 10p + 1 */
        seq_t term = big_pow(base, (seq_t)exps[i]);  /* (10p+1)^e */
        result *= term;
    }

    return result;
}
