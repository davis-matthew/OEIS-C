/*
 * A083267  Product of related numbers (counted in A073757) belonging to n;
 *          related = {divisor-set, RRS}: a(n) = A007955(n)*A001783(n).
 * https://oeis.org/A083267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007955(n) * A001783(n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq_t.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 1;
    if (n == 1)
        return 1;

    /* Prime factorisation of n */
    idx_t tmp = n;
    idx_t primes[64];
    idx_t exps[64];
    int pc = 0;

    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            idx_t cnt = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++cnt;
            }
            primes[pc] = p;
            exps[pc]  = cnt;
            ++pc;
        }
    }
    if (tmp > 1)
    {
        primes[pc] = tmp;
        exps[pc]   = 1;
        ++pc;
    }

    /* Number of divisors τ(n) = ∏ (e_i + 1) */
    idx_t tau = 1;
    for (int i = 0; i < pc; ++i)
        tau *= (exps[i] + 1);

    /* Product of all divisors: ∏ p_i^{e_i * τ/2} */
    seq_t prodDiv = 1;
    for (int i = 0; i < pc; ++i)
    {
        idx_t expPow = exps[i] * tau / 2; /* integer exponent */
        prodDiv *= pow_seq_t((seq_t)primes[i], expPow);
    }

    /* Product of reduced residue system modulo n */
    seq_t prodRed = 1;
    for (idx_t k = 1; k < n; ++k)
    {
        int coprime = 1;
        for (int i = 0; i < pc; ++i)
        {
            if (k % primes[i] == 0)
            {
                coprime = 0;
                break;
            }
        }
        if (coprime)
            prodRed *= (seq_t)k;
    }

    return prodDiv * prodRed;
}
