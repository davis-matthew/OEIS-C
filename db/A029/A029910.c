/*
 * A029910  Start with n; if prime, stop; repeatedly sum prime factors
 *          (counted with multiplicity) and add 1, until reach 1, 6 or a
 *          prime.
 * https://oeis.org/A029910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = n                if n == 1 or n == 6 or n is prime
   a(n) = a( sum_of_prime_factors_mult(n) + 1 ) otherwise
   (sum_of_prime_factors_mult(n) = Σ p_i * e_i for n = Π p_i^e_i) */

#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Special cases: 1, 6, and primes */
    if (n <= 1)          return 1;
    if (n == 6)          return 6;
    if (is_prime(n))     return (seq_t)n;

    /* Factor n and compute the sum of prime factors with multiplicity */
    uint64_t u = (uint64_t)n;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(u, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)primes[i] * (seq_t)exps[i];

    /* Next value: sum + 1 */
    return a((idx_t)(sum + 1));
}
