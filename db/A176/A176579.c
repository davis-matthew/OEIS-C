/*
 * A176579  Primes p that exactly one of 2p-1, 2p+1 is semiprime.
 * https://oeis.org/A176579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime p for which exactly one of 2p‑1, 2p+1 is
 *            semiprime
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------ */
/* Global state for prime table                                             */
static uint64_t *prime_arr = NULL;   /* array of primes up to prime_limit                    */
static size_t   prime_cnt = 0;       /* number of primes stored                               */
static idx_t    prime_limit = 0;     /* maximum value for which primes have been generated   */

/* ------------------------------------------------------------------ */
/* Ensure that primes are computed at least up to LIMIT                  */
static void ensure_primes_upto(idx_t limit)
{
    if (prime_limit >= limit)
        return;

    uint64_t *new_arr = NULL;
    size_t   new_cnt = 0;
    build_primes_upto(limit, &new_arr, &new_cnt);

    /* Free previous allocation if any */
    if (prime_arr)
        free(prime_arr);

    prime_arr  = new_arr;
    prime_cnt  = new_cnt;
    prime_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Test whether an unsigned 64‑bit integer is a semiprime                */
static bool is_semiprime_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    uint64_t temp = n;
    unsigned int factors = 0;

    for (size_t i = 0; i < prime_cnt && prime_arr[i] * prime_arr[i] <= temp; ++i)
    {
        uint64_t p = prime_arr[i];
        while (temp % p == 0)
        {
            ++factors;
            if (factors > 2)
                return false;
            temp /= p;
        }
    }
    if (temp > 1)   /* remaining prime factor */
        ++factors;

    return factors == 2;
}

/* ------------------------------------------------------------------ */
/* Check whether a prime p satisfies the property                      */
static bool qualifies(uint64_t p)
{
    uint64_t a = 2 * p - 1;
    uint64_t b = 2 * p + 1;
    bool sa = is_semiprime_uint64(a);
    bool sb = is_semiprime_uint64(b);
    return (sa ^ sb);   /* exactly one of them is semiprime */
}

/* ------------------------------------------------------------------ */
/* Public function required by the harness                               */
seq_t a(idx_t n)
{
    /* Sequence offset is 1; undefined behaviour for n==0 */
    if (n == 0)
        return (seq_t)0;

    /* Define an upper limit that comfortably covers
       the 10 000th term (~2 000 000)                            */
    const idx_t PRECOMPUTED_LIMIT = 2000000;

    ensure_primes_upto(PRECOMPUTED_LIMIT);

    /* Scan the prime table to find the n‑th qualifying prime    */
    size_t found = 0;
    uint64_t result = 0;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_arr[i];
        if (qualifies(p))
        {
            ++found;
            if (found == (size_t)n)
            {
                result = p;
                break;
            }
        }
    }

    /* If we did not find the term within the precomputed limit,
       (unlikely for n ≤ 10000) recompute with a larger limit.    */
    while (found < (size_t)n)
    {
        idx_t new_limit = prime_limit * 2;
        if (new_limit == 0)
            new_limit = 100000;
        ensure_primes_upto(new_limit);

        /* Restart search over the expanded table            */
        found = 0;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t p = prime_arr[i];
            if (qualifies(p))
            {
                ++found;
                if (found == (size_t)n)
                {
                    result = p;
                    break;
                }
            }
        }
    }

    return (seq_t)result;
}
