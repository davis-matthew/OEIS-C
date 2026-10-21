/*
 * A059172  Numbers k such that k/rad(k) > sqrt(k) where rad(k) is the largest
 *          squarefree number dividing k.
 * https://oeis.org/A059172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 923, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A059172:
 * the sequence of integers k such that k / rad(k) > sqrt(k),
 * equivalently k > rad(k)^2 where rad(k) is the product of
 * distinct prime divisors of k. */
#include <stdint.h>
#include <stdlib.h>

/* -------------------------------------------------------------------------
 * Small‑prime‑factor sieve for fast factorisation.
 * ------------------------------------------------------------------------- */
#define SIEVE_LIMIT 5000000U          /* covers the first 10 000 terms */

static uint32_t *spf = NULL;         /* smallest prime factor table */

static void build_sieve(void)
{
    if (spf != NULL) return;                      /* already built */

    spf = (uint32_t*)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    if (spf == NULL) return; /* out of memory – unlikely in the test harness */

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
    {
        if (spf[i] == 0)          /* i is prime */
        {
            spf[i] = i;
            if ((uint64_t)i * i <= SIEVE_LIMIT)
            {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

/* -------------------------------------------------------------------------
 * Compute rad(n) for n up to SIEVE_LIMIT using the sieve.
 * For larger n a simple trial division is used.
 * ------------------------------------------------------------------------- */
static inline uint64_t rad_of(uint64_t n)
{
    if (n <= SIEVE_LIMIT)
    {
        uint64_t rad = 1;
        while (n > 1)
        {
            uint32_t p = spf[(uint32_t)n];
            rad *= (uint64_t)p;
            while (n % p == 0) n /= p;
        }
        return rad;
    }

    /* trial division for very large n (not expected in the test range) */
    uint64_t rad = 1;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            rad *= p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) rad *= temp;
    return rad;
}

/* -------------------------------------------------------------------------
 * Sequence cache.  cache[i] holds a(i+1).  The cache is built on demand
 * when a(n) is requested.  It works for arbitrary access order.
 * ------------------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static uint64_t last_k = 1;          /* last integer inspected */

/* -------------------------------------------------------------------------
 * a(n) – OEIS A059172
 * ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* safety for invalid index */

    /* Build the sieve once. */
    build_sieve();

    /* Grow cache until we have at least n terms. */
    while (cache_len < (size_t)n)
    {
        ++last_k;
        if (last_k < 2) continue;     /* 1 is not in the sequence */

        uint64_t rad = rad_of(last_k);
        /* Compare using 64‑bit integers; rad^2 fits in 64 bits for our limits. */
        if (last_k > rad * rad)
        {
            cache = (seq_t*)realloc(cache, (cache_len + 1) * sizeof(seq_t));
            cache[cache_len++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
