/*
 * A069098  Number of minimal monic annihilator polynomials over the ring of
 *          integers modulo n.
 * https://oeis.org/A069098
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 1 if n is prime; Otherwise, let n = ∏ p_i^e_i. Let S(n)
 *            be the least m such that n | m! (Kempner function of n). For
 *            each prime factor p^e, define b(j) = min{ v_p(j!) , e } (0 ≤ j <
 *            S). Then a(n) = ∏ p_i ^(∑_{j=0}^{S-1} b(j))
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 73, u64 n <= 133,
 *            u128 n <= 261, big any n
 * Helpers:   big_pow, factor_u64, is_prime, min_k_for_prime_power, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"    /* is_prime(idx_t)   */
#include "../utils/big_pow.h"     /* big_pow(seq_t,seq_t)  */
#include "../utils/seq_mul.h"     /* seq_mul(seq_t,seq_t)  */
#include "../utils/factor_u64.h"  /* factor_u64(uint64_t po,uint64_t ex,size_t*) */
#include "../utils/min_k_for_prime_power.h"

seq_t a(idx_t n)
{
    /* n is at least 2; primes are handled directly */
    if (is_prime((idx_t)n))
        return (seq_t)1;

    /* factorize n into primes and exponents */
    uint64_t primes[64], exps[64];
    size_t pf = 0;
    factor_u64((uint64_t)n, primes, exps, &pf);

    /* compute the Kempner function S(n) = minimal m with n | m! */
    idx_t S = 0;
    for (size_t i = 0; i < pf; ++i)
    {
        idx_t p = primes[i];
        idx_t e = exps[i];
        idx_t k = min_k_for_prime_power(p, e, n);
        if (k > S) S = k;
    }

    /* accumulate product of N(S, p, e) over prime powers */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < pf; ++i)
    {
        uint64_t p = primes[i];
        idx_t    e = exps[i];

        /* sum_{j=0}^{S-1} min(v_p(j!), e) */
        idx_t sum_b = 0;
        idx_t v = 0; /* current exponent of p in j!   */
        for (idx_t j = 0; j < S; ++j)
        {
            if (j > 0)
            {
                /* add exponent of p in j   */
                idx_t t = j;
                while (t % p == 0) { ++v; t /= p; }
            }
            idx_t add = v;
            if (add > e) add = e;
            sum_b += add;
        }

        /* multiply by p^sum_b   */
        seq_t p_pow = big_pow((seq_t)p, (seq_t)sum_b);
        result = seq_mul(result, p_pow);
    }

    return result;
}
