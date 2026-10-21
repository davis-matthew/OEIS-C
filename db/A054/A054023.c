/*
 * A054023  Chowla function of n is not divisible by the number of divisors of
 *          n.
 * https://oeis.org/A054023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer m (m ≥ 1) such that (σ(m)-m-1) mod τ(m)
 *            ≠ 0, where σ is the sum of divisors of m and τ is the number of
 *            divisors of m
 * Range:     u8 n <= 161, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Smallest‑prime‑factor array and prime list.  They are rebuilt when the
   requested upper bound grows.                                                        */
static uint64_t *spf      = NULL;  /* spf[i] = smallest prime factor of i (i ≥ 2) */
static size_t   spf_size  = 0;
static uint64_t *prime_arr = NULL;  /* list of primes found so far */
static size_t   prime_cnt = 0;
static size_t   prime_cap = 0;

/* Ensure the sieve covers all integers up to 'limit'.  The function
   rebuilds the spf array and primes from scratch; this simple
   implementation is fast enough for the required range.                       */
static void ensure_spf(size_t limit)
{
    if (limit < 2) return;

    /* Grow the spf array if necessary. */
    if (spf == NULL)
    {
        spf = (uint64_t*)calloc(limit + 1, sizeof(uint64_t));
        spf_size = limit + 1;
    }
    else if (spf_size <= limit)
    {
        spf = (uint64_t*)realloc(spf, (limit + 1) * sizeof(uint64_t));
        memset(spf + spf_size, 0, (limit + 1 - spf_size) * sizeof(uint64_t));
        spf_size = limit + 1;
    }

    /* Initialise spf entries that are still zero. */
    for (size_t i = 2; i <= limit; ++i)
        if (spf[i] == 0)
            spf[i] = i;

    /* Mark composite numbers. */
    for (size_t i = 2; i * i <= limit; ++i)
    {
        if (spf[i] == i)                     /* i is prime */
        {
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                if (spf[j] == j)
                    spf[j] = i;
        }
    }

    /* Rebuild prime list. */
    free(prime_arr);
    prime_arr = NULL;
    prime_cnt = 0;
    prime_cap = 0;
    for (size_t i = 2; i <= limit; ++i)
    {
        if (spf[i] == i)
        {
            if (prime_cnt >= prime_cap)
            {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_arr = (uint64_t*)realloc(prime_arr, prime_cap * sizeof(uint64_t));
            }
            prime_arr[prime_cnt++] = i;
        }
    }
}

/* Test whether n satisfies the defining property; store the value
   in *res if true.  Returns 1 if satisfied, 0 otherwise. */
static int satisfies(uint64_t n, seq_t *res)
{
    uint64_t x = n;
    seq_t    sigma = 1;       /* product of geometric series terms */
    uint64_t tau   = 1;       /* number of divisors */

    while (x > 1)
    {
        uint64_t p   = spf[x];
        int      exp = 0;
        while (x % p == 0) { x /= p; ++exp; }

        /* Compute 1 + p + p² + … + p^exp */
        seq_t term = 1;
        seq_t p_pow = 1;
        for (int k = 1; k <= exp; ++k)
        {
            p_pow = p_pow * (seq_t)p;
            term  = term + p_pow;
        }
        sigma = sigma * term;
        tau   = tau * (uint64_t)(exp + 1);
    }

    seq_t C = sigma - 1 - (seq_t)n;
    if (C % (seq_t)tau != 0)
    {
        *res = (seq_t)n;
        return 1;
    }
    return 0;
}

/* Cache of sequence values. */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Next integer to examine.  Starts at 1. */
static uint64_t next_int = 1;

/* Return the n-th term of A054023 (1‑based index). */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        /* Ensure the sieve knows about 'next_int'. */
        ensure_spf(next_int);

        seq_t val;
        if (satisfies(next_int, &val))
        {
            /* Store the term in the cache.  Resize if needed. */
            if (cache_len >= cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = val;
        }
        ++next_int;
    }

    return cache[(size_t)n - 1];
}
