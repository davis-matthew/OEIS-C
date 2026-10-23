/*
 * A151932  a(n) = 2^(2*n)*(n!)^2*Product_{e_k} binomial(2*e_k, e_k) where 2n
 *          = Product p_k^e_k is the prime factorization of 2n.
 * https://oeis.org/A151932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n) * (n!)^2 * ∏_{e_k} binomial(2*e_k, e_k) where 2*n
 *            = ∏ p_k^e_k Offset: 1 (a(1) is the first term)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   binom, factor_u64, factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factorial.h"
#include "../utils/pow2.h"
#include "../utils/binom.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined below offset */
        return 0;

    /* 2^(2*n) */
    seq_t result = pow2((idx_t)((idx_t)2 * n));

    /* (n!)^2 */
    seq_t fn = factorial(n);
    result *= fn;
    result *= fn;

    /* factorise 2*n and multiply binom(2*e_k, e_k) for each exponent e_k */
    uint64_t two_n = ((uint64_t)n) << 1;          /* 2*n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 64;                              /* maximum number of distinct primes we can hold */
    factor_u64(two_n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t bin = binom((idx_t)(2u * exps[i]), (idx_t)exps[i]);
        result *= bin;
    }

    return result;
}
