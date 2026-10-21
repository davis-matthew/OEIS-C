/*
 * A237517  Pisano period of n^2 divided by Pisano period of n.
 * https://oeis.org/A237517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n^2)/pi(n) where pi(m) is the Pisano period modulo m
 * Range:     u8 n <= 9, u16 n <= 1252, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, pisano_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* uses only the gcd() utility for computing lcm of periods */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/pisano_prime.h"

/* fast exponentiation for small exponents (all exponents here are < 30) */
static inline seq_t pow_seq_small(seq_t base, int exp)
{
    seq_t r = 1;
    for (int i = 0; i < exp; ++i) r *= base;
    return r;
}

/* lcm(a,b) using gcd */
static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return a / g * b;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* factorise n by trial division (n <= 10^4 in most tests) */
    uint64_t primes[64];
    int exp[64];
    size_t cnt = 0;
    uint64_t tmp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        int e = 0;
        while (tmp % p == 0)
        {
            tmp /= p;
            ++e;
        }
        if (e)
        {
            primes[cnt] = p;
            exp[cnt]    = e;
            ++cnt;
        }
    }
    if (tmp > 1)
    {
        primes[cnt] = tmp;
        exp[cnt]    = 1;
        ++cnt;
    }

    seq_t pi_n  = 1;          /* Pisano period of n */
    seq_t pi_n2 = 1;          /* Pisano period of n^2 */

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p   = primes[i];
        int e       = exp[i];
        seq_t pi_p   = pisano_prime(p); /* period of the prime itself */

        /* period of p^e */
        seq_t term_n  = pow_seq_small((seq_t)p, e - 1) * pi_p;
        pi_n = lcm_seq(pi_n, term_n);

        /* period of p^(2e) */
        seq_t term_n2 = pow_seq_small((seq_t)p, 2 * e - 1) * pi_p;
        pi_n2 = lcm_seq(pi_n2, term_n2);
    }

    /* the required ratio */
    return pi_n2 / pi_n;
}
