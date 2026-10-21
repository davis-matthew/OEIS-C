/*
 * A079636  Smallest number whose reciprocal fits in the square-root gap of
 *          consecutive primes.
 * https://oeis.org/A079636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(1/(√p(n+1) – √p(n)))
 * Range:     u8 n <= 53, u16 n <= 6576, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: the identity 1/(√b – √a) = (√a+√b)/(b-a) is used. */
/* For integer arithmetic we test successive k for the inequality */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*----------------------------------------------------------------------*/
/*  Prime generation (simple sieve up to a fixed limit)                 */
/*  We generate all primes ≤ 200 000 – this is more than enough for      */
/*  the first 10 000 terms (the 10 001st prime is 104 729).             */
/*----------------------------------------------------------------------*/

#define SIEVE_LIMIT 200000               /* upper bound for primes */

/* The sieve and prime list are created only once. */
static uint8_t *sieve_table   = NULL;    /* 0 -> prime, 1 -> composite */
static seq_t *prime_list      = NULL;    /* list of primes as seq_t */
static size_t prime_count     = 0;       /* number of primes stored */
static bool primes_prepared   = false;

/* build the sieve, populate prime_list, and record prime_count */
static void prepare_primes(void)
{
    if (primes_prepared)
        return;

    /* allocate and fill the sieve */
    sieve_table = (uint8_t *)calloc(SIEVE_LIMIT + 1, 1);
    if (!sieve_table)
        abort();

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (!sieve_table[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve_table[j] = 1;

    /* count primes to allocate the list */
    size_t cnt = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!sieve_table[i])
            ++cnt;

    prime_list = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!prime_list)
        abort();

    /* fill the list */
    size_t pos = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!sieve_table[i])
            prime_list[pos++] = (seq_t)i;

    /* record */
    prime_count   = cnt;
    primes_prepared = true;
}

/* make sure we have at least (need+1) primes (zero‑based index).      */
static void ensure_primes(idx_t need)
{
    prepare_primes();
    if (prime_count <= (size_t)need)
        /* the chosen limit is insufficient; abort (should not happen) */
        abort();
}

/*----------------------------------------------------------------------*/
/*  Sequence implementation                                            */
/*----------------------------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0)    /* sequence is 1‑based; caller never asks for 0 */
        return (seq_t)0;

    /* we need primes p(n) and p(n+1) -> indices n-1 and n in 0-based list */
    ensure_primes(n);

    seq_t p1 = prime_list[n - 1];    /* p(n)   */
    seq_t p2 = prime_list[n];        /* p(n+1) */

    uint64_t d   = (uint64_t)p2 - (uint64_t)p1;   /* prime gap, fits in 64‑bit */
    uint64_t sum = (uint64_t)p1 + (uint64_t)p2;
    uint64_t prod = (uint64_t)p1 * (uint64_t)p2;   /* product fits in 64‑bit for our limits */

    /* find smallest k with (k*d)^2 - sum  ≥  2 * √(prod)   (equivalent to k ≥ ceil( (√p1+√p2)/d )) */
    uint64_t k = 1;
    while (true)
    {
        uint64_t K  = k * d;               /* k*d   */
        uint64_t K2 = K * K;               /* (k*d)^2 */
        int64_t  A  = (int64_t)K2 - (int64_t)sum;   /* K^2 - (p1+p2) */

        if (A >= 0)
        {
            /* check A^2 ≥ 4*prod   – all fits in 64‑bit for the tested limits */
            uint64_t lhs = A * (uint64_t)A;
            uint64_t rhs = 4 * prod;
            if (lhs >= rhs)
                return (seq_t)k;
        }
        ++k;
    }
}
/*=====================================================================*/
