/*
 * A165982  Primes that are 4 plus the sum of three consecutive primes.
 * https://oeis.org/A165982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime of the form 4 + (p_k + p_{k+1} + p_{k+2}),
 *            where (p_k) are the consecutive primes. The terms are generated
 *            in ascending order of the corresponding primes p_k. Each term is
 *            tested for primality with is_prime_big
 * Range:     u8 n <= 8, u16 n <= 346, u32 n <= 347, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime_big, seq_alloc, seq_from_str,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t) */
#include "../utils/seq_from_str.h"      /* seq_t seq_from_str(const char *) */
#include "../utils/seq_alloc.h"         /* seq_t *seq_alloc(size_t) */
#include "../utils/seq_realloc.h"       /* seq_t *seq_realloc(seq_t *,size_t,size_t) */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t, uint64_t **, size_t *) */

/* --------------------------------------------------------------------- */
/*  Local state                                                        */
/* --------------------------------------------------------------------- */
static seq_t*  terms      = NULL;   /* cached sequence values            */
static size_t terms_cap   = 0;      /* allocated capacity of terms       */
static size_t terms_count = 0;      /* number of terms stored            */

static uint64_t* primes       = NULL;   /* array of primes up to primes_limit */
static size_t    primes_count = 0;      /* number of primes stored            */
static idx_t     primes_limit  = 0;     /* current largest prime value        */

static idx_t current_prime_index = 0;    /* start index of next triple         */

/* --------------------------------------------------------------------- */
/*  Helper: grow the terms array if necessary                           */
/* --------------------------------------------------------------------- */
static void add_term(seq_t val)
{
    if (terms_cap == 0 || terms_count == terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
        seq_t *new_arr = seq_realloc(terms, terms_cap, new_cap);
        if (!new_arr) exit(EXIT_FAILURE);
        terms      = new_arr;
        terms_cap  = new_cap;
    }
    terms[terms_count++] = val;
}

/* --------------------------------------------------------------------- */
/*  Helper: ensure that primes up to at least limit are available       */
/*  The helper may rebuild the entire prime table when the limit is
   increased.  This is safe because all previous terms have already
   been generated from triples that were fully contained in the old
   table.                                                               */
/* --------------------------------------------------------------------- */
static void ensure_primes_to_limit(idx_t limit)
{
    if (limit <= primes_limit) return;          /* already covered          */

    if (primes) free(primes);                   /* discard old table        */

    build_primes_upto(limit, &primes, &primes_count);
    primes_limit = limit;
}

/* --------------------------------------------------------------------- */
/*  Ensure we have enough primes for the next triple                      */
/* --------------------------------------------------------------------- */
static void ready_next_triple(void)
{
    /* initial table: 100 000 is ample for the first few terms          */
    if (primes_limit == 0) {
        ensure_primes_to_limit(100000ULL);
    }

    /* If the current starting index would run past the table, extend it */
    while (current_prime_index + 2 >= primes_count) {
        idx_t new_limit = primes_limit * 2;
        if (new_limit <= primes_limit)                 /* overflow guard */ 
            new_limit = primes_limit + 100000ULL;
        ensure_primes_to_limit(new_limit);
    }
}

/* --------------------------------------------------------------------- */
/*  The required OEIS sequence function                                  */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined for n < 1 */

    /* generate terms until we reach n */
    while (terms_count < n) {
        ready_next_triple();

        uint64_t sum = primes[current_prime_index]     +
                       primes[current_prime_index + 1] +
                       primes[current_prime_index + 2];

        /* form the candidate number 4 + sum and convert to seq_t   */
        char buf[32];
        snprintf(buf, sizeof buf, "%llu",
                 (unsigned long long)(sum + 4ULL));
        seq_t candidate = seq_from_str(buf);

        /* keep the candidate if it is prime */
        if (is_prime_big(candidate))
            add_term(candidate);

        ++current_prime_index;
    }

    return terms[n - 1];          /* 1‑based index: a(1) is first term */
}
