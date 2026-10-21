/*
 * A085343  Number of primes between sigma(n) and phi(n) inclusive.
 * https://oeis.org/A085343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(σ(n)) - pi(φ(n)) where σ is sum‑of‑divisors and φ is
 *            Euler’s totient. The function factorises n by trial division
 *            using a cached list of primes and then uses the helper
 *            compute_pi from the utils library
 * Range:     u8 n <= 659, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_pi.h"
#include "../utils/build_primes_upto.h"

extern seq_t a(idx_t n)
{
    /* n is >=1; guard just in case */
    if (n == 0)
        return 0;

    /* static cache of primes, built once */
    static bool init = false;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;

    if (!init) {
        /* 100 000 is more than enough for factorising all n≤10 000
           and keeps the sieve cheap.  The build_primes_upto helper
           allocates the array and sets primes_cnt. */
        build_primes_upto(100000, &primes, &primes_cnt);
        init = true;
    }

    uint64_t nn = (uint64_t)n;
    uint64_t t   = nn;
    uint64_t sigma = 1;
    uint64_t phi   = 1;

    /* trial division with cached primes */
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= t; ++i) {
        uint64_t p = primes[i];
        if (t % p != 0)
            continue;

        /* count exponent of p */
        int exp = 0;
        while (t % p == 0) {
            t   /= p;
            exp += 1;
        }

        /* compute σ contribution: (p^(exp+1)-1)/(p-1) */
        uint64_t p_pow = 1;
        for (int k = 0; k < exp + 1; ++k)
            p_pow *= p;
        sigma *= (p_pow - 1) / (p - 1);

        /* compute φ contribution */
        if (exp == 1)
            phi *= (p - 1);
        else {
            uint64_t p_pow_exp1 = 1;
            for (int k = 0; k < exp - 1; ++k)
                p_pow_exp1 *= p;
            phi *= p_pow_exp1 * (p - 1);
        }
    }

    /* remaining prime factor, if any */
    if (t > 1) {
        sigma *= (1 + t);
        phi   *= (t - 1);
    }

    /* use the provided prime counting routine */
    seq_t pi_sigma = compute_pi((idx_t)sigma);
    seq_t pi_phi   = compute_pi((idx_t)phi);

    return pi_sigma - pi_phi;
}
