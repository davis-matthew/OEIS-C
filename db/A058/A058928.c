/*
 * A058928  Denominators of series related to triangular cacti.
 * https://oeis.org/A058928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator((2*n+1)^(n-1)/(2^n * n!))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Cache of primes up to the largest n seen */
static uint64_t *prime_list = NULL;
static size_t      prime_cnt  = 0;
static idx_t       prime_limit = 0;

static void
ensure_primes(idx_t n)
{
    if (n <= prime_limit)
        return;
    free(prime_list);
    build_primes_upto(n, &prime_list, &prime_cnt);
    prime_limit = n;
}

/* Legendre's formula: exponent of prime p in n! */
static inline idx_t
legendre(idx_t n, idx_t p)
{
    idx_t exp = 0;
    for (idx_t pp = p; pp <= n; pp *= p)
    {
        exp += n / pp;
        if (pp > n / p)   /* avoid overflow of pp */
            break;
    }
    return exp;
}

/* fast exponentiation for seq_t */
static inline seq_t
pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1)
            res *= base;
        if (exp > 1)
            base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Ensure primes up to n for factorisation and Legendre */
    ensure_primes(n);

    /* Allocate exponent array for primes in 2*n+1 */
    uint32_t *e_of_p = (uint32_t *)calloc(prime_cnt, sizeof(uint32_t));
    if (!e_of_p)
        return 0;               /* allocation failed */

    /* Factor 2*n+1 only using primes <= n (others would have no effect) */
    uint64_t tmp = 2ULL * n + 1;
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= n && tmp > 1; ++i)
    {
        uint64_t p = prime_list[i];
        if (tmp % p == 0)
        {
            uint32_t cnt = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++cnt;
            }
            e_of_p[i] = cnt;
        }
    }

    /* Start with the denominator: 2^n * n! */
    idx_t exp2 = n + legendre(n, 2);
    seq_t res = pow_seq((seq_t)2, exp2);

    /* Process odd primes p <= n */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        if (p <= 2)
            continue;                     /* skip prime 2 */
        if (p > n)
            break;                        /* remaining primes not needed */

        idx_t v_p = legendre(n, p);      /* exponent of p in n! */
        if (v_p == 0)
            continue;                     /* p does not divide n! */

        idx_t cancel = (idx_t)(n > 0 ? (n - 1) * e_of_p[i] : 0);  /* (n-1)*v_p(2n+1) */
        idx_t exp_p = v_p;

        if (cancel < exp_p)
            exp_p = exp_p - cancel;       /* remaining exponent after cancellation */
        else
            exp_p = 0;                    /* fully cancelled */

        if (exp_p > 0)
            res *= pow_seq((seq_t)p, exp_p);
    }

    free(e_of_p);
    return res;
}
