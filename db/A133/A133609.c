/*
 * A133609  Numbers k such that k, k+2 and k+4 are consecutive semiprimes.
 * https://oeis.org/A133609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A133609:  Numbers k such that k, k+2 and k+4 are consecutive semiprimes.
    The function computes the n‑th such k (a(1) = 183).              */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* #include build_spf utility */
#include "../utils/build_spf.h"

/* --------------------------------------------------------------------- */
/*  Static state and helpers                                               */
/* --------------------------------------------------------------------- */

static idx_t *spf_arr = NULL;      /* Array of smallest prime factors    */
static idx_t  spf_limit = 0;        /* Max index covered by spf_arr       */

static seq_t *cache      = NULL;    /* Cache of found k's                 */
static size_t cache_len  = 0;        /* Number of cached terms             */
static size_t cache_cap  = 0;        /* Allocated capacity of cache        */

static idx_t next_num    = 2;        /* Next integer to examine            */
static idx_t prev1       = 0;        /* Most recent semiprime              */
static idx_t prev2       = 0;        /* Second most recent semiprime       */
static bool   have_prev1 = false;    /* Flag for existence of prev1        */
static bool   have_prev2 = false;    /* Flag for existence of prev2        */

/* Ensure that spf_arr is computed up to at least 'limit'.              */
static void
ensure_spf(idx_t limit)
{
    if (spf_limit >= limit)
        return;                     /* already sufficient */

    /* free old array if present */
    if (spf_arr) {
        free(spf_arr);
        spf_arr = NULL;
    }

    /* build new SPF array up to 'limit'                                 */
    build_spf(limit, &spf_arr);
    spf_limit = limit;
}

/* Test whether n is a semiprime (product of exactly two primes).      */
static bool
is_semiprime(idx_t n)
{
    if (n < 4)                     /* smallest semiprime is 4   */
        return false;

    /* n <= spf_limit must hold */
    idx_t p = spf_arr[n];
    if (p == 0)                    /* safety check               */
        return false;

    idx_t q = n / p;                /* the co‑factor              */
    return (q > 1) && (spf_arr[q] == q);   /* q must be prime        */
}

/* Grow the cache array when needed.                                      */
static void
grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                  /* unrecoverable             */
    cache     = new_cache;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Public routine: a(idx_t n)                                            */
/* --------------------------------------------------------------------- */

seq_t
a(idx_t n)
{
    if (n == 0)                     /* undefined for n<1, but avoid UB   */
        return 0;

    /* Ensure cache contains at least n entries.                          */
    if (cache_len < n) {

        /* Initialise sieve if this is the first call.                   */
        if (spf_limit == 0)
            ensure_spf(1000000);    /* initial sieve size               */

        while (cache_len < n) {
            /* Expand sieve if we run out of SPF data.                   */
            if (next_num > spf_limit) {
                idx_t new_limit = spf_limit ? spf_limit * 2 : 1000000;
                ensure_spf(new_limit);
            }

            /* Examine next number.                                      */
            if (is_semiprime(next_num)) {
                /* Check for a triple with the two previous semiprimes. */
                if (have_prev1) {
                    if (have_prev2) {
                        if (prev2 == next_num - 4 && prev1 == next_num - 2) {
                            /* Found a valid k = prev2 */
                            if (cache_len == cache_cap)
                                grow_cache();
                            cache[cache_len++] = (seq_t)prev2;
                        }
                    }
                    /* Shift the window of previous semiprimes.          */
                    prev2 = prev1;
                    have_prev2 = true;
                }
                prev1 = next_num;
                have_prev1 = true;
            }

            ++next_num;             /* advance to next integer              */
        }
    }

    return cache[n - 1];
}
