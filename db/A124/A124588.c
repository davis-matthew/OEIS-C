/*
 * A124588  Primes p such that q - p <= 2, where q is the next prime after p.
 * https://oeis.org/A124588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that the next prime q satisfies q -
 *            p ≤ 2
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   alloc_seq_array, build_primes_upto, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (i.e. 2 together with the lower members of each twin‑prime pair.) */

#include <stddef.h>
#include <stdint.h>

/* Utility functions provided by the build system */
#include "../utils/build_primes_upto.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq.h"

/* cache for the sequence values */
static seq_t *terms   = NULL;
static size_t term_count = 0;
static idx_t  sieve_limit = 0;

/* rebuild the cache using a sieve up to LIMIT (prime value) */
static void recompute_terms(idx_t limit)
{
    uint64_t *primes = NULL;
    size_t   prime_cnt = 0;

    build_primes_upto(limit, &primes, &prime_cnt);
    if (prime_cnt < 2) {
        free(primes);
        return;
    }

    /* count qualifying primes (p where next prime q differs by ≤2) */
    size_t found = 0;
    for (size_t i = 1; i < prime_cnt; ++i) {
        if (primes[i] - primes[i-1] <= 2U)
            ++found;
    }

    /* allocate space for the new terms */
    seq_t *new_terms = alloc_seq_array(found);
    size_t idx = 0;
    for (size_t i = 1; i < prime_cnt; ++i) {
        if (primes[i] - primes[i-1] <= 2U) {
            new_terms[idx++] = (seq_t)primes[i-1];
        }
    }

    /* replace old cache if any */
    if (terms)
        free_seq(terms);
    terms       = new_terms;
    term_count  = found;
    sieve_limit = limit;

    free(primes);
}

/* ensure that at least N terms are available in the cache */
static void ensure_n(idx_t n_needed)
{
    if (term_count >= (size_t)n_needed)
        return;

    idx_t limit = (sieve_limit == 0) ? 200000 :
                  (sieve_limit * 2);

    /* increase the sieve limit until enough terms are found */
    while (term_count < (size_t)n_needed) {
        if (limit < sieve_limit)   /* overflow protection */
            limit = sieve_limit;

        recompute_terms(limit);
        if (term_count >= (size_t)n_needed)
            break;

        /* double the limit for the next try */
        size_t new_limit = (size_t)limit * 2;
        if (new_limit <= (size_t)limit)  /* overflow check */
            new_limit = (size_t)-1;      /* maximum value */
        limit = new_limit;
    }
}

/* public API required by the build system */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for indices before the offset */

    ensure_n(n);
    return terms[n - 1];
}
