/*
 * A013956  a(n) = sigma_8(n), the sum of the 8th powers of the divisors of n.
 * https://oeis.org/A013956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sigma_8(n) = Sum_{d|n} d^8 = product_{p^e||n} (1 + p^8 +
 *            p^(2*8) + ... + p^(e*8))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* fact_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/big_pow.h"     /* seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* OEIS offset 1: a(1)=1. For n==0 we return 0 as a guard. */
    if (n == 0) return 0;
    if (n == 1) return (seq_t)1;

    /* Factorize n into prime powers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p   = (seq_t)primes[i];
        seq_t p8  = big_pow(p, (seq_t)8);   /* p^8 */
        seq_t term = (seq_t)1;              /* 1 + p^8 + ... */
        seq_t pow   = (seq_t)1;             

        for (size_t j = 1; j <= (size_t)exps[i]; ++j)
        {
            pow  *= p8;                      /* p^(8*j) */
            term += pow;
        }

        result *= term;
    }

    return result;
}
