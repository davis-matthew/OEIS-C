/*
 * A217863  a(n) = phi(lcm(1,2,3,...,n)), where phi is Euler's totient
 *          function.
 * https://oeis.org/A217863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(lcm(1,2,…,n)) = ∏_{p≤n} (p−1) · ∏_{p^k≤n, k≥2} p
 *            (product over all primes p≤n; multiply (p−1) once and multiply p
 *            for each power p^k (k≥2) that is ≤n)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 24, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)1;  /* φ(1)=1 for a(1) but n≥1 always in tests */

    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];
        result *= (seq_t)(p - 1);

        idx_t power = (idx_t)p * p;   /* first power p^2 */
        while (power <= n)
        {
            result *= (seq_t)p;
            power *= (idx_t)p;        /* next power p^(k+1) */
        }
    }

    free(primes);
    return result;
}
