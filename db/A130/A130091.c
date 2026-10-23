/*
 * A130091  Numbers having in their canonical prime factorization mutually
 *          distinct exponents.
 * https://oeis.org/A130091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 4351, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th integer whose prime factorisation has all
   distinct exponents. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/floor_sqrt.h"      /* floor_sqrt(seq_t) */
#include "../utils/build_primes_upto.h"   /* build_primes_upto(limit,&primes,&cnt) */

/* ----------------- Prime cache -------------------------------------- */
static uint64_t *prime_list  = NULL;   /* primes as 64‑bit values   */
static size_t   prime_cnt   = 0;       /* number of primes          */
static idx_t    prime_limit = 0;       /* largest prime generated   */

static void
ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    if (prime_list != NULL)
        free(prime_list);

    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* ----------------- Test for distinct‑exponent property ---------------- */
static bool
is_special(seq_t val)
{
    if (val == (seq_t)1)      /* 1 is considered special */
        return true;
    if (val == (seq_t)0)
        return false;

    /* Work in 64‑bit arithmetic for the range of numbers we encounter. */
    uint64_t n = (uint64_t)val;
    uint64_t tmp = n;
    bool seen_exp[65] = { false };   /* indices 0…64 correspond to exponent 1…65 */

    idx_t sqrt_lim = (idx_t)floor_sqrt((seq_t)n);
    ensure_primes_up_to(sqrt_lim);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > sqrt_lim)
            break;
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            if (e > 64)            /* exponent too large to track */
                return false;
            if (seen_exp[(size_t)e - 1])
                return false;     /* duplicate exponent found */
            seen_exp[(size_t)e - 1] = true;
            sqrt_lim = (idx_t)floor_sqrt((seq_t)n);
        }
    }

    /* Remaining factor > 1 contributes exponent 1. */
    if (n > 1) {
        if (seen_exp[0])
            return false;         /* exponent 1 already seen */
        seen_exp[0] = true;
    }

    return true;
}

/* ----------------- Sequence cache ------------------------------------- */
static seq_t *cache       = NULL;  /* cached terms                */
static size_t cache_len   = 0;     /* number of cached terms       */
static size_t cache_cap   = 0;     /* allocated capacity           */
static seq_t last_checked = (seq_t)2; /* next candidate to test */

/* ----------------- Main sequence function ------------------------------ */
seq_t a(idx_t n)
{
    if (cache_len == 0) {          /* initialise cache with term 1 */
        cache_cap = 16;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
        last_checked = (seq_t)2;
    }

    while (cache_len < n) {
        if (is_special(last_checked)) {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp)
                    exit(1);
                cache = tmp;
            }
            cache[cache_len++] = last_checked;
        }
        ++last_checked;
    }

    return cache[n - 1];
}
