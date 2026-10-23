/*
 * A130586  Row sums of triangle A130585.
 * https://oeis.org/A130586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k|(n+1)} φ(k)·2^(k-1) (OEIS A130586)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   factor_u64, phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/phi.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t m = n + 1;               /* m = n+1, the number whose divisors are used */

    /* Factorization of m: primes[] holds the prime factors, exps[] the exponents. */
    uint64_t primes[16], exps[16];
    size_t cnt;
    factor_u64(m, primes, exps, &cnt);

    /* Generate all divisors of m. 256 divisors is more than enough for the domain. */
    idx_t divisors[256];
    size_t div_len = 1;
    divisors[0] = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        idx_t p = primes[i];
        idx_t e = exps[i];
        size_t current_len = div_len;

        idx_t p_pow = 1;
        for (idx_t exp = 1; exp <= e; ++exp)
        {
            p_pow *= p;
            for (size_t j = 0; j < current_len; ++j)
                divisors[div_len++] = divisors[j] * p_pow;
        }
    }

    /* Sum over all divisors k of m: φ(k)·2^(k-1) */
    seq_t sum = 0;
    for (size_t i = 0; i < div_len; ++i)
    {
        idx_t k = divisors[i];
        seq_t phi_k   = phi(k);
        seq_t pow_term = pow2(k - 1);
        sum += phi_k * pow_term;
    }

    return sum;
}
