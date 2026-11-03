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
 * Validated: standard, pass, 82 terms through n = 1888 (2026-11-01)
 * Helpers:   factor_u64, ipow, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"
#include "../utils/is_prime.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static idx_t min_k_for_prime_power(idx_t p, idx_t e, idx_t upper)
{
    idx_t lo = 1, hi = upper;
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t cnt = 0, t = mid;
        while (t) { t /= p; cnt += t; }
        if (cnt >= e) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

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
        seq_t p_pow = ipow((seq_t)p, (seq_t)sum_b);
        result = seq_mul(result, p_pow);
    }

    return result;
}
