/*
 * A234640  Odd numbers n for which sigma(sigma(sigma(n))) is odd.
 * https://oeis.org/A234640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th odd integer m such that sigma(sigma(sigma(m)))
 *            is odd, where sigma is the sum of divisors function
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* The following utility provides a fast prime generator. */
#include "../utils/build_primes_upto.h"

/* ---------- Precomputed data ---------- */

static const idx_t PRECOMP_LIMIT = 100000;   /* small limit for cache lookup */
static const idx_t PRIME_LIMIT   = 1000000;  /* primes up to this bound */

/* Cached sigma values for 1..PRECOMP_LIMIT. */
static seq_t *sigma_cache = NULL;   /* size PRECOMP_LIMIT+1 */

/* Prime table. */
static uint64_t *prime_table = NULL;
static size_t   prime_cnt    = 0;

/* ---------- Helper: factorisation and sigma for uint64_t ---------- */

/* Compute sigma(n) for uint64_t n.  Overflows wrap modulo 2^64, preserving parity. */
static uint64_t sigma_uint64(uint64_t n)
{
    /* initialise prime table and sigma cache lazily */
    if (sigma_cache == NULL) {
        /* allocate and compute sigma cache */
        sigma_cache = (seq_t *)calloc((size_t)PRECOMP_LIMIT + 1, sizeof(seq_t));
        if (!sigma_cache) abort();  /* out of memory */
        for (idx_t i = 1; i <= PRECOMP_LIMIT; ++i)
            for (idx_t j = i; j <= PRECOMP_LIMIT; j += i)
                sigma_cache[(size_t)j] += (seq_t)i;

        /* build prime table */
        build_primes_upto((idx_t)PRIME_LIMIT, &prime_table, &prime_cnt);
    }

    if (n <= PRECOMP_LIMIT)
        return (uint64_t)sigma_cache[(size_t)n];

    uint64_t result = 1;
    uint64_t temp   = n;

    for (size_t k = 0; k < prime_cnt && prime_table[k] * prime_table[k] <= temp; ++k) {
        uint64_t p = prime_table[k];
        if (temp % p == 0) {
            uint64_t part  = 1;    /* sigma of p^e */
            uint64_t power = 1;    /* p^e */
            while (temp % p == 0) {
                temp   /= p;
                power  *= p;
                part   += power;
            }
            result *= part;
        }
    }
    if (temp > 1) /* remaining prime factor > sqrt(original) */
        result *= (1 + temp);

    return result;
}

/* Wrapper that returns seq_t (needed by sequence function). */
static seq_t sum_of_divisors_uint64(uint64_t n)
{
    return (seq_t)sigma_uint64(n);
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, behaviour for n==0 is irrelevant */

    idx_t  found   = 0;
    seq_t  result  = 0;

    /* iterate over odd candidates */
    for (uint64_t cand = 1; found < n; cand += 2ULL) {
        uint64_t s1 = sigma_uint64(cand);
        uint64_t s2 = sigma_uint64(s1);
        uint64_t s3 = sigma_uint64(s2);

        if ((s3 & 1ULL) != 0ULL) {   /* sigma^3(cand) is odd */
            ++found;
            if (found == n) {
                result = (seq_t)cand;
                break;
            }
        }
    }

    return result;
}
