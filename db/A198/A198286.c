/*
 * A198286  a(n) = Sum_{d|n} (A053143(d) or smallest square divisible by d).
 * https://oeis.org/A198286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A198286
 *
 * a(n) is a multiplicative function.  For a prime power p^e,
 *      a(p^e) = 1 + 2 * (p^(e+2) - p^2) / (p^2 - 1)          if e is even
 *              = (1 + p^2) * (p^(e+1) - 1) / (p^2 - 1)        if e is odd
 *
 * The implementation factors n with the utility `factor_u64` and
 * multiplies the contributions of each prime power.
 * It works for any seq_t (u8, u16, u32, u64, u128 or big integer) and
 * any idx_t (uint64_t).
 *
 * Complexity: O(number of prime factors of n) per call.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/factor_u64.h"

static seq_t a_prime_power(uint64_t p, unsigned e)
{
    seq_t p_seq = (seq_t)p;
    seq_t p2    = p_seq * p_seq;          /* p^2 */
    seq_t denom = p2 - (seq_t)1;          /* p^2 - 1 */

    if ((e & 1U) == 0)   /* even exponent */
    {
        seq_t num = pow_uint(p, (idx_t)(e + 2)) - p2;   /* p^(e+2) - p^2 */
        seq_t part = (seq_t)2 * (num / denom);
        return part + (seq_t)1;
    }
    else                /* odd exponent */
    {
        seq_t num = pow_uint(p, (idx_t)(e + 1)) - (seq_t)1; /* p^(e+1) - 1 */
        seq_t part = (p2 + (seq_t)1) * (num / denom);
        return part;
    }
}

seq_t a(idx_t n)
{
    if (n <= (idx_t)1)      /* a(1)=1, guard for n=0 */
        return (seq_t)1;

    /* Allocate temporary storage for factorization (max 64 prime factors). */
    size_t const max_factors = 64;
    uint64_t *primes   = (uint64_t *)malloc(max_factors * sizeof(uint64_t));
    uint64_t *exps     = (uint64_t *)malloc(max_factors * sizeof(uint64_t));
    if (!primes || !exps) {
        /* OOM – return 0 to avoid crash; rarely occurs */
        return (seq_t)0;
    }
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        result *= a_prime_power(primes[i], (unsigned)exps[i]);
    }

    free(primes);
    free(exps);
    return result;
}
