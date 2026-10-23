/*
 * A129656  Infinitary abundant numbers: integers for which A126168 (n)>n, or
 *          equivalently for which A049417 (n)>2n.
 * https://oeis.org/A129656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 2800, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Infinitary abundant numbers: integers n for which the sum of
   infinitary divisors sigma_inf(n) exceeds 2·n.
   The function returns the n‑th such integer (a(1)=24). */

/* Note:  A single cache exchanges the expensive
   computation of sigma_inf(·) for a data store that grows only when
   a new term is requested.  The sieve of smallest prime factors is
   lazily extended to cover the current search range.  The code
   satisfies both the C11 and C++17 requirements. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper that builds the smallest–prime‑factor array up to a limit. */
#include "../utils/build_spf.h"

/* ------------------------------------------------------------------ */
/* Global state for the sieve and the cache of infinitary abundant
   numbers. */
static idx_t *spf = NULL;                /* array of smallest prime factors */
static size_t spf_limit = 0;              /* size of the sieve array */
static idx_t sieve_limit = 0;             /* current upper bound of the sieve */

static seq_t *abund_cache = NULL;        /* cached infinitary abundant numbers */
static size_t cache_len = 0;              /* number of stored terms */
static size_t cache_cap = 0;              /* allocated capacity of abund_cache */

static idx_t next_test = 2;               /* next candidate to check */

/* ------------------------------------------------------------------ */
/* Ensure that the spf array contains factors up to LIMIT. */
static void ensure_spf(idx_t limit)
{
    if (spf != NULL && limit <= spf_limit)
        return;                          /* already sufficient */

    if (spf != NULL) {
        free(spf);                       /* free the old array */
        spf = NULL;
    }

    build_spf(limit, &spf);              /* helper allocates the array */
    spf_limit = (size_t)limit;
}

/* ------------------------------------------------------------------ */
/* Compute the sum of infinitary divisors of n using the spf array.
   For n in the sieve, n ≤ spf_limit. */
static seq_t sigma_inf(idx_t n)
{
    seq_t prod = 1;                      /* running product of prime factors */
    idx_t m = n;

    while (m > 1) {
        idx_t p = spf[m];
        int exp = 0;

        /* factorise p^exp from m */
        while (m % p == 0) {
            m /= p;
            exp++;
        }

        /* term for prime p with exponent exp:
           product over bits of exp of (1 + p^{2^k}) */
        seq_t term = 1;
        seq_t p_pow = (seq_t)p;          /* p^(2^0) initially */

        int e = exp;
        while (e > 0) {
            if (e & 1)
                term = term * (1 + p_pow);

            e >>= 1;
            if (e)
                p_pow = p_pow * p_pow;   /* p_pow becomes p^{2^{k+1}} */
        }

        prod = prod * term;
    }
    return prod;
}

/* ------------------------------------------------------------------ */
/* Grow the cache until it holds at least N entries. */
static void ensure_cache(size_t N)
{
    if (cache_len >= N)
        return;                          /* already enough entries */

    /* if no sieve has yet been built, start with a reasonable size */
    if (sieve_limit == 0) {
        sieve_limit = 300000ULL;
        ensure_spf(sieve_limit);
    }

    /* allocate or enlarge the cache array */
    if (cache_cap == 0) {
        cache_cap = 256;
        abund_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len >= cache_cap) {
        cache_cap *= 2;
        abund_cache = (seq_t *)realloc(abund_cache, cache_cap * sizeof(seq_t));
    }

    /* search for additional infinitary abundant numbers */
    while (cache_len < N) {
        if (next_test > sieve_limit) {   /* extend the sieve if needed */
            sieve_limit *= 2;            /* simple exponential growth */
            ensure_spf(sieve_limit);
        }

        seq_t s = sigma_inf(next_test);
        seq_t twice = (2 * (seq_t)next_test);
        if (s > twice) {
            /* store the term */
            if (cache_len >= cache_cap) {       /* just in case */
                cache_cap *= 2;
                abund_cache = (seq_t *)realloc(abund_cache,
                                               cache_cap * sizeof(seq_t));
            }
            abund_cache[cache_len++] = (seq_t)next_test;
        }
        next_test++;
    }
}

/* ------------------------------------------------------------------ */
/* Public entry point: a(n) returns the n‑th infinitary abundant number. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                          /* undefined for n==0 */

    ensure_cache((size_t)n);
    return abund_cache[(size_t)(n - 1)];
}
