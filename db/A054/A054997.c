/*
 * A054997  Integers that can be expressed as the sum of consecutive primes in
 *          exactly 2 ways.
 * https://oeis.org/A054997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3942, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate primes up to LIMIT, count for every integer ≤ LIMIT
   how many representations it has as a sum of consecutive primes.
   The numbers with exactly two representations are collected in
   ascending order.  The LIMIT is doubled until the list contains at
   least the requested index n.  The function stores all data in
   static tables so that calls in arbitrary order are efficient. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Helper: simple sieve up to 'limit'.
   The caller must free the returned array. */
static int *sieve_primes(idx_t limit, size_t *cnt)
{
    if (limit < 2u) {
        *cnt = 0;
        return NULL;
    }

    unsigned char *sieve = (unsigned char *)calloc((size_t)limit + 1, 1);
    if (!sieve) {
        *cnt = 0;
        return NULL;
    }

    size_t p;
    for (p = 2; p * p <= limit; ++p)
        if (!sieve[p]) {
            for (size_t k = p * p; k <= limit; k += p)
                sieve[k] = 1;
        }

    size_t prim_cnt = 0;
    for (p = 2; p <= limit; ++p)
        if (!sieve[p]) ++prim_cnt;

    int *pr = (int *)malloc(prim_cnt * sizeof(int));
    if (!pr) {
        free(sieve);
        *cnt = 0;
        return NULL;
    }
    size_t i = 0;
    for (p = 2; p <= limit; ++p)
        if (!sieve[p]) pr[i++] = (int)p;

    free(sieve);
    *cnt = prim_cnt;
    return pr;
}

/* Static cache for primes, counts and resulting sequence */
static int    *prime_cache      = NULL;
static size_t prime_cache_cnt   = 0;
static idx_t   prime_limit      = 0;

static int    *rep_cache        = NULL;  /* rep_cache[x] = ways to write x as
                                           sum of consecutive primes (0..2+) */
static idx_t   rep_limit        = 0;

static seq_t  *seq_cache        = NULL;
static idx_t   seq_cache_len    = 0;

/* Internal routine: rebuild tables up to new_limit > current rep_limit */
static void rebuild_tables(idx_t new_limit)
{
    if (new_limit <= rep_limit) return;          /* already done */

    /* Generate primes up to new_limit */
    size_t pc;
    int *pr = sieve_primes(new_limit, &pc);
    if (!pr) return;                     /* out of memory – silently fail */

    /* Allocate/create representation array */
    int *new_rep = (int *)calloc((size_t)new_limit + 1, sizeof(int));
    if (!new_rep) {
        free(pr);
        return;
    }

    /* Count consecutive‑prime sums */
    for (size_t i = 0; i < pc; ++i) {
        seq_t sum = 0;
        for (size_t j = i; j < pc; ++j) {
            sum += (seq_t)pr[j];
            if (sum > (seq_t)new_limit) break;
            new_rep[(size_t)sum]++;
        }
    }

    /* Build list of numbers having exactly two representations */
    idx_t new_seq_len = 0;
    for (idx_t v = 2; v <= new_limit; ++v)
        if (new_rep[(size_t)v] == 2) ++new_seq_len;

    seq_t *new_seq = (seq_t *)malloc(new_seq_len * sizeof(seq_t));
    if (!new_seq) {
        free(pr);
        free(new_rep);
        return;
    }
    idx_t pos = 0;
    for (idx_t v = 2; v <= new_limit; ++v)
        if (new_rep[(size_t)v] == 2) new_seq[pos++] = (seq_t)v;

    /* Replace global tables */
    free(prime_cache);
    if (rep_cache) free(rep_cache);
    free(seq_cache);

    prime_cache   = pr;
    prime_cache_cnt = pc;
    rep_cache     = new_rep;
    rep_limit     = new_limit;
    seq_cache     = new_seq;
    seq_cache_len = new_seq_len;
}

/* ------------------------- public interface ------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* safety – OEIS offset is 1 */

    /* Ensure cache contains at least n terms.  Start with a modest bound
       and double until sufficient. */
    idx_t need = n;
    idx_t limit = rep_limit;
    if (limit == 0) limit = 2000;             /* initial limit */
    while (seq_cache_len < need) {
        limit *= 2;
        rebuild_tables(limit);
        if (seq_cache_len >= need) break;
        /* if rebuild fails (memory) we abort gracefully */
        if (limit > (idx_t)1e9) break;
    }

    if (seq_cache_len < need) return 0;       /* out of memory */
    return seq_cache[n - 1];                  /* 1‑based index */
}
