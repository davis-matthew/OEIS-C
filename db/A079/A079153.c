/*
 * A079153  Primes p such that both p-1 and p+1 have at most 3 prime factors,
 *          counted with multiplicity; i.e., primes p such that bigomega(p-1)
 *          <= 3 and bigomega(p+1) <= 3, where bigomega(n) = A001222(n).
 * https://oeis.org/A079153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 98, u32 n <= 1100, u64 n <= 1100,
 *            u128 n <= 1100, big any n
 * Helpers:   bigomega, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A079153: primes p such that bigomega(p‑1) ≤ 3 and bigomega(p+1) ≤ 3. */
/* The function searches primes in ascending order, caching results so it
   works for any ordering of calls. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper prototypes provided by the framework */
#include "../utils/bigomega.h"          /* seq_t bigomega(idx_t n); */
#include "../utils/build_primes.h"      /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */

static seq_t *seq_cache     = NULL;   /* stored sequence values              */
static size_t seq_size      = 0;      /* number of terms cached              */
static size_t seq_cap       = 0;      /* current capacity of seq_cache       */

static idx_t *prime_list    = NULL;   /* array of primes up to current_limit*/
static size_t prime_cnt     = 0;      /* number of primes in prime_list      */
static size_t prime_next    = 0;      /* index of next prime to test         */
static idx_t  current_limit = 0;      /* highest bound used for primes       */
static bool   primes_ready  = false;  /* flag that primes have been built    */

static const idx_t INITIAL_LIMIT = 60000000ULL;  /* 60 million primes ≈ 10 k terms */

/* Rebuild the prime list up to 'limit'.  The old list is freed. */
static void rebuild_primes(idx_t limit)
{
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }
    build_primes(limit, &prime_list, &prime_cnt);
    prime_next   = 0;
    current_limit = limit;
    primes_ready = true;
}

/* Ensure at least 'n' terms of the sequence have been computed. */
static void ensure_sequence(idx_t n)
{
    /* Build initial prime list if not done yet. */
    if (!primes_ready) {
        rebuild_primes(INITIAL_LIMIT);
    }

    /* Expand the sequence until we have n terms. */
    while (seq_size < (size_t)n) {
        /* If the prime list is exhausted, double the bound. */
        if (prime_next >= prime_cnt) {
            rebuild_primes(current_limit * 2);
            continue;
        }

        idx_t p = prime_list[prime_next++];
        if (bigomega(p - 1) <= (seq_t)3 && bigomega(p + 1) <= (seq_t)3) {
            /* Grow cache if necessary. */
            if (seq_size == seq_cap) {
                size_t new_cap = seq_cap ? (seq_cap << 1) : 4;
                seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                seq_cap   = new_cap;
            }
            seq_cache[seq_size++] = (seq_t)p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* Undefined for n < 1 – return 0. */
        return 0;

    if (!primes_ready || seq_size < (size_t)n)
        ensure_sequence(n);

    return seq_cache[n - 1];
}
