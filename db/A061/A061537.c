/*
 * A061537  Product of the unitary divisors of n: a(n) = Product_{d|n,
 *          gcd(d,n/d) = 1} d.
 * https://oeis.org/A061537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(2^(ω(n)-1)) where ω(n) = number of distinct prime
 *            factors of n
 * Range:     u8 n <= 17, u16 n <= 29, u32 n <= 209, u64 n <= 329,
 *            u128 n <= 2309, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 1)
        return (seq_t)1;

    /* Factor n and count distinct prime factors */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);   /* cnt >= 1 for n > 1 */

    /* Exponent of n : 2^(cnt-1) */
    idx_t pow_exp = (idx_t)1 << (cnt - 1);

    /* Compute n^pow_exp by binary exponentiation */
    seq_t base = (seq_t)n;
    seq_t res  = 1;
    while (pow_exp)
    {
        if (pow_exp & 1ULL)
            res *= base;
        base *= base;
        pow_exp >>= 1ULL;
    }

    return res;
}
