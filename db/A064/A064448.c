/*
 * A064448  a(n) = gcd(n^n, phi(n^n)).
 * https://oeis.org/A064448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n^n, φ(n^n))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   gcd_seq_local, pow_seq_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = ∏ p_j^(n·e_j − 1) · gcd(∏ p_j , ∏ (p_j − 1)) */

#include <stdint.h>
#include "../utils/gcd_seq_local.h"
#include "../utils/pow_seq_local.h"

/* main sequence function */
seq_t a(idx_t n)
{
    /* factorization of n ( trial division, enough for n up to ~1e4 ) */
    uint64_t primes[64];
    int exps[64];
    int cnt = 0;

    uint64_t temp = (uint64_t)n;

    if (temp % 2 == 0)
    {
        primes[cnt] = 2;
        exps[cnt] = 0;
        while ((temp & 1) == 0)
        {
            temp >>= 1;
            exps[cnt]++;
        }
        cnt++;
    }

    for (uint64_t p = 3; p <= temp / p; p += 2)
    {
        if (temp % p == 0)
        {
            primes[cnt] = p;
            exps[cnt] = 0;
            while (temp % p == 0)
            {
                temp /= p;
                exps[cnt]++;
            }
            cnt++;
        }
    }

    if (temp > 1)
    {
        primes[cnt] = temp;
        exps[cnt] = 1;
        cnt++;
    }

    seq_t product_pow = 1;   /* ∏ p_j^(n·e_j − 1) */
    seq_t prodP      = 1;   /* ∏ p_j                     */
    seq_t prodQ      = 1;   /* ∏ (p_j − 1)                */

    seq_t n_seq = (seq_t)n; /* cast once */

    for (int i = 0; i < cnt; ++i)
    {
        /* exponent n·e_j − 1 (fits in int for typical n) */
        int exp = (int)(n_seq * exps[i] - 1);

        /* compute p_j^exp */
        seq_t p_pow = pow_seq_local((seq_t)primes[i], exp);

        product_pow *= p_pow;

        prodP *= (seq_t)primes[i];
        prodQ *= (seq_t)(primes[i] - 1);
    }

    seq_t gcdPQ = gcd_seq_local(prodP, prodQ);
    return product_pow * gcdPQ;
}
