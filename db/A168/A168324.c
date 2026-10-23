/*
 * A168324  Number of distinct permutations of the list of prime factors of n
 *          (with multiplicity), where a(1)=0.
 * https://oeis.org/A168324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product over prime factors p_i of binom(s, e_i), where s
 *            is the cumulative sum of exponents up to the i‑th prime (s = e_1
 *            + e_2 + … + e_i). a(1) = 0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   binom, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/binom.h"       /* binom */

seq_t a(idx_t n)
{
    if (n == 1)                  /* special case: no prime factors */
        return (seq_t)0;

    uint64_t primes[64];         /* storage for prime factors */
    uint64_t exps[64];           /* corresponding exponents */
    size_t cnt = 0;              /* number of distinct primes */

    /* factorize n (n fits in uint64_t) */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    idx_t total = 0;             /* cumulative count of prime factors so far */

    for (size_t i = 0; i < cnt; ++i)
    {
        total += (idx_t)exps[i];                 /* add the i‑th multiplicity */
        result *= binom(total, (idx_t)exps[i]);  /* multinomial increment */
    }

    return result;
}
