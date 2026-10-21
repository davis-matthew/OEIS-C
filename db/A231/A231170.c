/*
 * A231170  Semiprimes with equal number of even and odd digits.
 * https://oeis.org/A231170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 811, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   equal_even_odd_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate the n‑th semiprime whose decimal representation
   contains an equal number of even and odd digits.
   The implementation uses a smallest‑prime‑factor sieve,
   extending the sieve and the cache lazily until it has
   at least n terms.  This works for any seq_t (arbitrary‑
   precision or fixed‑size integer) and any idx_t (uint64_t). */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/equal_even_odd_digits.h"

/* ------------------------------------------------------------------ */
/* Helper: test for equal number of even and odd decimal digits.      */

/* ------------------------------------------------------------------ */
/* Global state for caching the sequence and the SPF sieve.            */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache      = NULL;  /* array of terms */
static size_t  cache_len     = 0;     /* current number of terms   */
static size_t  cache_cap     = 0;     /* allocated capacity        */

static idx_t *spf            = NULL;  /* array of smallest prime factors */
static idx_t  sieve_lim      = 0;     /* maximum index of the SPF array built so far */
static idx_t  processed_lim  = 0;     /* largest number processed for the sequence */

/* ------------------------------------------------------------------ */
/* Ensure the cache can hold at least 'need' entries.                  */
/* ------------------------------------------------------------------ */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    seq_cache = new_cache;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Build or extend the SPF sieve up to 'new_lim'.                      */
/* ------------------------------------------------------------------ */
static void build_spf_upto(idx_t new_lim)
{
    if (new_lim <= sieve_lim)
        return; /* already built */

    /* Reallocate SPF array. */
    idx_t *new_spf = (idx_t *)realloc(spf, (new_lim + 1) * sizeof(idx_t));
    if (!new_spf) abort();
    spf = new_spf;

    /* Initialize new entries. */
    for (idx_t i = sieve_lim + 1; i <= new_lim; ++i)
        spf[i] = 0;

    /* Sieve to fill SPF values. */
    for (idx_t i = 2; i <= new_lim; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            if (i * i <= new_lim) {
                for (idx_t j = i * i; j <= new_lim; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }

    sieve_lim = new_lim;
}

/* ------------------------------------------------------------------ */
/* Generate sequence values from processed_lim+1 up to 'new_lim'      */
/* and append them to the cache.                                      */
/* ------------------------------------------------------------------ */
static void extend_sequence(idx_t new_lim)
{
    build_spf_upto(new_lim);

    for (idx_t n = processed_lim + 1; n <= new_lim; ++n) {
        if (n < 4) continue;  /* first semiprime is 4 */

        idx_t p = spf[n];
        if (p == 0) continue;  /* not prime, skip */

        idx_t q = n / p;
        if (q < 2 || spf[q] != q)
            continue;          /* q must also be prime for a semiprime */

        seq_t x = (seq_t)n;
        if (equal_even_odd_digits(x)) {
            ensure_cache_capacity(cache_len + 1);
            seq_cache[cache_len++] = x;
        }
    }

    processed_lim = new_lim;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined; offset is 1 */

    /* Ensure we have enough terms to answer request. */
    if (processed_lim == 0) {
        extend_sequence(500000); /* initial thrust */
    }
    while (cache_len < n) {
        idx_t new_lim = sieve_lim * 2;
        if (new_lim == 0)
            new_lim = 1000; /* safeguard */
        extend_sequence(new_lim);
    }

    return seq_cache[n - 1];
}
