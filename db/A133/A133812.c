/*
 * A133812  Numbers that are primally tight and have weakly descending powers.
 * https://oeis.org/A133812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 6901, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A133812 – Numbers that are primally tight and have weakly
 *  descending powers.
 *
 *  The sequence is the set of integers n such that
 *      n = p1^e1 * p2^e2 * … * pk^ek,
 *  where p1 < p2 < … < pk are consecutive primes and e1 ≥ e2 ≥ … ≥ ek.
 *
 *  This function returns the n-th term of the sequence (a(1)=1).  It
 *  generates terms by testing consecutive integers and keeping the ones
 *  that satisfy the prime‑consecutive, weakly decreasing exponent
 *  constraints.  The implementation uses a small dynamic cache and an
 *  Sieve of Eratosthenes for pre‑computing smallest prime factors and
 *  the list of primes necessary for factorisation.
 *
 *  The function is re‑entrant and works correctly for any order of
 *  calls, caching previously generated terms.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/build_spf.h"
#include "../utils/build_primes_upto.h"

/* ----  global cached data  ---- */
static seq_t *cache = NULL;          /* cached sequence values      */
static size_t cache_cap = 0;         /* capacity of cache           */
static idx_t  cache_len = 0;         /* number of values cached     */

static idx_t limit = 0;              /* current maximal integer
                                       covered by SPF / prime data */
static uint64_t *spf = NULL;         /* smallest prime factor array */
static uint64_t *primes = NULL;      /* array of primes up to limit */
static size_t prime_cnt = 0;         /* number of primes            */
static int *prime_idx = NULL;        /* map prime → consecutive index (−1 = unknown) */

/* ----  helper: compute SPF and primes up to new_limit  ---- */
static void expand_limit(idx_t new_limit)
{
    if (new_limit <= limit)
        return;

    /* free previous data */
    free(spf);
    free(primes);
    free(prime_idx);

    /* build new SPF array */
    build_spf(new_limit, &spf);

    /* build list of primes up to new_limit */
    build_primes_upto(new_limit, &primes, &prime_cnt);

    /* build mapping prime → consecutive index */
    prime_idx = (int *)calloc((size_t)new_limit + 1, sizeof(int));
    if (!prime_idx) exit(1);            /* allocation failure */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p <= (uint64_t)new_limit)
            prime_idx[(size_t)p] = (int)i;
    }

    limit = new_limit;
}

/* ----  helper: test whether n satisfies the definition  ---- */
static bool is_primally_tight(idx_t n)
{
    if (n == 1)
        return true;

    idx_t tmp = n;
    int prev_idx = -1;
    idx_t prev_exp = 0;

    while (tmp > 1) {
        idx_t p = spf[tmp];            /* smallest prime factor */

        /* count exponent of p */
        idx_t exp = 0;
        do {
            tmp /= p;
            ++exp;
        } while (tmp % p == 0);

        /* p must be known (i.e., <= limit) */
        if (p > limit)
            return false;

        int idx = prime_idx[(size_t)p];
        if (idx < 0)                /* not a prime in the table */
            return false;

        if (prev_idx != -1) {       /* check consecutiveness */
            if (idx != prev_idx + 1)
                return false;

            /* weakly decreasing exponents */
            if (exp > prev_exp)
                return false;
        }

        prev_idx  = idx;
        prev_exp  = exp;
    }

    return true;
}

/* ----  main sequence function  ---- */
seq_t a(idx_t n)
{
    if (n == 0)                      /* graceful handling of offset 0 */
        return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 128;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* initial pre‑computation */
    if (limit == 0)
        expand_limit(1000);

    static idx_t next_candidate = 1;  /* next integer to test */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* ensure the pre‑computed range covers the current candidate */
        if (next_candidate > limit) {
            idx_t new_limit = limit * 2;
            if (new_limit <= limit)          /* overflow – go to max value */
                new_limit = UINT64_MAX;
            if (new_limit < next_candidate)
                new_limit = next_candidate;
            expand_limit(new_limit);
        }

        /* test the current candidate */
        if (is_primally_tight(next_candidate)) {
            /* store it if it is a valid term */
            if (cache_len == cache_cap) {    /* grow cache */
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
