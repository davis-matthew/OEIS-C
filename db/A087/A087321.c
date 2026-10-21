/*
 * A087321  Smallest multiple of n which is a perfect power (at least a
 *          square) of a squarefree number.
 * https://oeis.org/A087321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product_{p|n} p^K, where K = max(2, max exponent of p in
 *            n)
 * Range:     u8 n <= 16, u16 n <= 159, u32 n <= 1343, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   intpow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/intpow_seq.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)n;   /* a(1)=1, a(0) not used */

    /* factorization of n */
    uint64_t primes[32];
    int exps[32];
    int cnt = 0;
    uint64_t temp = (uint64_t)n;

    /* factor 2 */
    int exp = 0;
    while ((temp & 1ULL) == 0)
    {
        ++exp;
        temp >>= 1;
    }
    if (exp > 0)
    {
        primes[cnt] = 2;
        exps[cnt]   = exp;
        ++cnt;
    }

    /* odd factors */
    for (uint64_t p = 3; p <= temp / p; p += 2)
    {
        if (temp % p == 0)
        {
            exp = 0;
            while (temp % p == 0)
            {
                ++exp;
                temp /= p;
            }
            primes[cnt] = p;
            exps[cnt]   = exp;
            ++cnt;
        }
    }
    if (temp > 1)
    {
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* compute K = max exponent, but at least 2 */
    int maxExp = 0;
    for (int i = 0; i < cnt; ++i)
        if (exps[i] > maxExp) maxExp = exps[i];
    unsigned K = maxExp >= 2 ? (unsigned)maxExp : 2U;

    /* compute product of primes^K */
    seq_t result = 1;
    for (int i = 0; i < cnt; ++i)
        result *= intpow_seq((seq_t)primes[i], K);

    return result;
}
