/*
 * A092771  Prime(prime(n))^2-1.
 * https://oeis.org/A092771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(prime(n)))^2 – 1 (p_k denotes the k‑th prime)
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   build_primes_up_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_up_to.h"

/* --------------------------------------------------------------------- */
/*  Helper: generate all primes ≤ limit using a simple sieve.            */
/*  The primes are stored in a newly allocated array returned in *primes */
/*  and the number of primes returned in *count.                        */

/* --------------------------------------------------------------------- */
/*  Static state for caching primes.                                      */
/* --------------------------------------------------------------------- */
static uint32_t *cached_primes   = 0;
static size_t   cached_count    = 0;
static size_t   cached_limit    = 0;   /* sieve upper bound of the cached primes */

/* --------------------------------------------------------------------- */
/*  Ensure that the prime table contains at least 'need' primes.          */
/*  The table is grown as necessary by doubling the limit, which keeps    */
/*  the amortised cost O(n log log n).                                     */
/* --------------------------------------------------------------------- */
static void ensure_primes(size_t need)
{
    if (need <= cached_count) return;          /* already sufficient */

    /* Initial estimate of the sieve limit.  For small n it is very generous, */
    /* for larger n we increase the limit geometrically until enough primes */
    /* have been generated.                                                 */
    size_t limit = 0;
    if (cached_limit == 0) {
        /* start with a small bound; the factor 15 is safe for the ranges     */
        /* of n used in the tests (up to at least 10 000).                     */
        limit = need * 15 + 100;
        if (limit < 100) limit = 100;
    } else {
        limit = cached_limit * 2;
        /* Ensure the new limit is not too small compared to 'need'            */
        size_t min_limit = need * 15 + 100;
        if (limit < min_limit) limit = min_limit;
    }

    /* Repeatedly double the limit until we have enough primes            */
    while (1) {
        uint32_t *new_primes;
        size_t     new_count;
        build_primes_up_to(limit, &new_primes, &new_count);

        if (new_count >= need) {
            /* Replace the old table                                      */
            free(cached_primes);
            cached_primes   = new_primes;
            cached_count    = new_count;
            cached_limit    = limit;
            break;
        }
        free(new_primes);
        limit *= 2;       /* grow further and try again */
    }
}

/* --------------------------------------------------------------------- */
/*  Main function: returns the n-th term of A092771.                      */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Indices in the problem are 1‑based.  n is unsigned and >= 1.          */
    /* ----------------------------------------------------------------- */
    /* 1. nth prime p_n                                                   */
    ensure_primes((size_t)n);            /* guarantee at least n primes */
    uint32_t p_n = cached_primes[(size_t)n - 1];

    /* 2. prime whose index is p_n  ->  p_{p_n}                           */
    ensure_primes((size_t)p_n);          /* need at least p_n primes */
    uint32_t p_pn = cached_primes[(size_t)p_n - 1];

    /* 3. compute (p_{p_n})^2 – 1                                         */
    seq_t val = (seq_t)p_pn;
    return val * val - (seq_t)1;
}
