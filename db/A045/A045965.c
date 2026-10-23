/*
 * A045965  a(1)=2; if n = Product p_i^e_i, n > 1, then a(n) = Product
 *          p_{i+1}^e_i.
 * https://oeis.org/A045965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = ∏ next_prime(p_i)^{e_i} where n = ∏ p_i^{e_i} and a(1) =
 *            2
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factorize n */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t nextp = next_prime(primes[i]);
        seq_t power = seq_pow((seq_t)nextp, (idx_t)exps[i]);
        result = seq_mul(result, power);
    }

    return result;
}
