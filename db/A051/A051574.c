/*
 * A051574  a(n) = number of k, 1<=k<=n, such that (n*k) divides
 *          binomial(n,k).
 * https://oeis.org/A051574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k | 1≤k≤n : n*k │ binomial(n,k) }
 * Range:     u8 n <= 1876, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine counts all k in [1,n] for which the exponent of every prime
   divising (n*k) does not exceed its exponent in C(n,k), using Legendre's
   formula for prime exponents in factorials.                                       */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Smallest‑prime‑factor table (built up to the largest n seen so far). */
static uint32_t *spf = NULL;
static idx_t    spf_size = 0;

/* -------------------------------------------------------------------- */
/* Build or extend the spf table so that spf[i] is defined for 1<=i<=limit. */
static void
ensure_spf(idx_t limit)
{
    if (limit <= spf_size)
        return;                     /* already built */

    uint32_t *tmp = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    if (!tmp)                     /* allocation failed – abort */
        abort();
    spf = tmp;

    /* initialise new entries with their own value */
    for (idx_t i = spf_size + 1; i <= limit; ++i)
        spf[i] = (uint32_t)i;

    /* sieve the new range */
    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (spf[i] == (uint32_t)i)          /* i is prime */
        {
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
        }
    }
    spf_size = limit;
}

/* Legendre's formula: exponent of prime p in n!                           */
static inline idx_t
exp_in_factorial(idx_t n, idx_t p)
{
    idx_t e = 0;
    for (idx_t t = n; t; t /= p)
        e += t / p;
    return e;
}

/* -------------------------------------------------------------------- */
#define MAX_PRIMES 32

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Ensure we can factorise all numbers up to n.                         */
    ensure_spf(n);

    /* Factor n ----------------------------------------------------------*/
    size_t m = 0;                                     /* distinct primes of n */
    uint32_t primes_n[MAX_PRIMES];
    idx_t    exp_n  [MAX_PRIMES];
    idx_t tmp = n;
    while (tmp > 1 && m < MAX_PRIMES)
    {
        uint32_t p = spf[tmp];
        idx_t    e = 0;
        while (tmp % p == 0)
        {
            tmp /= p;
            ++e;
        }
        primes_n[m] = p;
        exp_n  [m] = e;
        ++m;
    }

    seq_t result = 0;

    /* Main loop over k --------------------------------------------------*/
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Factor k ----------                                       */
        uint32_t primes_k[MAX_PRIMES];
        idx_t    exp_k  [MAX_PRIMES];
        size_t   mk = 0;
        idx_t rem = k;
        while (rem > 1 && mk < MAX_PRIMES)
        {
            uint32_t p = spf[rem];
            idx_t    e = 0;
            while (rem % p == 0)
            {
                rem /= p;
                ++e;
            }
            primes_k[mk] = p;
            exp_k  [mk] = e;
            ++mk;
        }

        bool ok = true;

        /* Check primes that divide n (they may or may not appear in k). */
        for (size_t i = 0; i < m && ok; ++i)
        {
            uint32_t p = primes_n[i];
            idx_t    e_in_k = 0;
            for (size_t j = 0; j < mk; ++j)
            {
                if (primes_k[j] == p)
                {
                    e_in_k = exp_k[j];
                    break;
                }
            }

            idx_t e_bin = exp_in_factorial(n,   p)
                        - exp_in_factorial(k,   p)
                        - exp_in_factorial(n-k,p);
            idx_t need   = exp_n[i] + e_in_k;
            if (e_bin < need)
                ok = false;
        }

        if (!ok)
            continue;

        /* Check primes that divide k but not n. */
        for (size_t j = 0; j < mk && ok; ++j)
        {
            uint32_t p = primes_k[j];
            /* skip if p already handled above (i.e., p divides n) */
            bool seen_in_n = false;
            for (size_t i = 0; i < m; ++i)
                if (primes_n[i] == p)
                {
                    seen_in_n = true;
                    break;
                }
            if (seen_in_n)
                continue;

            idx_t e_bin = exp_in_factorial(n,   p)
                        - exp_in_factorial(k,   p)
                        - exp_in_factorial(n-k,p);
            if (e_bin < exp_k[j])
                ok = false;
        }

        if (ok)
            ++result;
    }

    return result;
}
