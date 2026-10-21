/*
 * A072437  Numbers with no prime factors of form 4*k+3.
 * https://oeis.org/A072437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose prime factors are not congruent to 3 modulo 4. */
/* The function enumerates integers, filters by prime‑factor tests, and caches
   the results so that calls in any order are efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_spf.h"
#include "../utils/seq_realloc.h"

static seq_t *seq      = NULL; /* cache of found terms */
static size_t seq_len  = 0;     /* number of terms stored      */
static size_t seq_cap  = 0;     /* capacity of seq[]           */

static idx_t *spf       = NULL; /* smallest prime factor array    */
static idx_t  spf_limit = 0;    /* largest n for which spf is valid */

static idx_t  last_checked = 0; /* largest integer examined so far */

/* Ensure that spf covers integers up to up_to (inclusive).  Rebuild the
   sieve if necessary. */
static void
ensure_spf_capacity(idx_t up_to)
{
    if (up_to <= spf_limit)
        return;

    idx_t new_limit = spf_limit ? spf_limit * 2 : 128;
    while (new_limit < up_to)
        new_limit *= 2;

    idx_t *new_spf = NULL;
    build_spf(new_limit, &new_spf);

    if (spf != NULL)
        free(spf);
    spf       = new_spf;
    spf_limit = new_limit;
}

/* Return true if num has a prime factor ≡ 3 (mod 4). */
static bool
has_bad_prime_factor(idx_t num)
{
    idx_t tmp = num;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        if ((p & 3ULL) == 3ULL)        /* p ≡ 3 (mod 4) */
            return true;
        while (tmp % p == 0)
            tmp /= p;
    }
    return false;  /* all prime factors are 2 or ≡ 1 (mod 4) */
}

/* Generate terms until we have at least n terms cached. */
static void
expand_sequence(idx_t n)
{
    /* Loop until we have n terms. */
    for (idx_t cand = last_checked + 1; seq_len < (size_t)n; ++cand) {
        ensure_spf_capacity(cand);

        if (!has_bad_prime_factor(cand)) {
            /* Grow the cache if necessary. */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
                seq_t *new_seq = seq_realloc(seq, seq_cap, new_cap);
                seq          = new_seq;
                seq_cap      = new_cap;
            }
            seq[seq_len++] = (seq_t)cand;
        }
        last_checked = cand;
    }
}

/* Public interface: return a(n), 1‑based index. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* undefined for offset 1; safe placeholder */

    if ((size_t)n > seq_len)
        expand_sequence(n);

    return seq[n - 1];
}
