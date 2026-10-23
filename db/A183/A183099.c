/*
 * A183099  a(n) = sum of powerful divisors d (excluding 1) of n.
 * https://oeis.org/A183099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of powerful divisors d of n, with d > 1 (a powerful
 *            number has each prime factor occurring with exponent ≥ 2)
 * Range:     u8 n <= 143, u16 n <= 8882, u32 n <= 8882, u64 n <= 8882,
 *            u128 n <= 8882, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 and for prime n the sum is 0 */
    if (n < 2) return 0;

    /* factor n */
    size_t cnt = 0;
    /* the number of distinct prime factors of n is ≤ 64 for n < 2^64 */
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* product over all primes of the sum of allowed powers */
    seq_t total = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t exp = exps[i];
        if (exp < 2) continue;                     /* no powerful term for this prime */

        seq_t p = (seq_t)primes[i];

        /* sum_{k=2}^{exp} p^k  =  p^2 * (p^{exp-1} - 1) / (p - 1)  */
        seq_t p_pow_exp_minus1 = seq_pow(p, (idx_t)(exp - 1));
        seq_t numerator   = p_pow_exp_minus1 - 1;
        seq_t denom       = p - 1;
        seq_t sum_ge_2    = (p * p * numerator) / denom;

        /* total sum for this prime factor, including the 1-case */
        seq_t prime_sum = 1 + sum_ge_2;

        total = total * prime_sum;
    }

    /* exclude the divisor 1 from the sum */
    return total - 1;
}
