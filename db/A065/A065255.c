/*
 * A065255  Consider the sequence b(k) such that b(k) and sigma(b(k)) end with
 *          the same digit in base 10. Sequence gives values of b(k) such that
 *          b(k)/k = 10.
 * https://oeis.org/A065255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 63, u32 n <= 249, u64 n <= 249,
 *            u128 n <= 249, big any n
 * Helpers:   cache_append
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A065255 – a(n) is the nth integer x such that
 *   (i)   sigma(x) ≡ x (mod 10)          (the last decimal digit of x
 *       and its divisor–sum coincide),
 *   (ii)  x is a multiple of 10,
 *   (iii) let k be the index of x in the sequence of all numbers
 *         satisfying (i); then x/k = 10   (i.e. k = x/10).
 *
 * The function scans successive integers, keeps a running count of the
 * numbers that satisfy (i), and records those that also satisfy (ii)
 * and (iii).  The cache is retained between calls so that any order of
 * requests is handled quickly.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cache_append.h"

/* forward declarations */
static seq_t sigma_of_uint64(idx_t n, uint32_t *spf);
static void build_spf(idx_t limit, uint32_t **out, idx_t *limit_out);
static void cache_append(seq_t val, seq_t **cache, size_t *len, size_t *cap);

/* --------------------------------------------------------------------- */
/* Main routine requested by the test harness.                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Cache of the a(n) sequence. */
    static seq_t *seq_cache = NULL;
    static size_t seq_len  = 0;   /* number of terms already computed   */
    static size_t seq_cap  = 0;   /* allocated capacity of the cache    */

    /* State for the enumeration of integers. */
    static idx_t next_candidate = 1;   /* next integer to examine          */
    static idx_t b_index = 0;          /* running index of b(k) numbers    */

    /* State for computing σ(n). */
    static uint32_t *spf = NULL;       /* smallest prime factor array      */
    static idx_t spf_limit = 0;        /* maximal integer prepared          */

    /* ---------------------------------------------------------------- */
    /* Grow the cache if more terms are required.                       */
    /* ---------------------------------------------------------------- */
    if (seq_len < n) {
        /* Allocate an empty cache if necessary. */
        if (seq_cache == NULL) {
            seq_cap  = 64;
            seq_cache = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        }

        /* Keep extending until we have at least n terms. */
        while (seq_len < n) {
            /* Ensure the spf array covers 'next_candidate'. */
            if (next_candidate > spf_limit) {
                idx_t new_limit = spf_limit ? spf_limit * 2 : 100000;
                if (new_limit < next_candidate * 2) new_limit = next_candidate * 2;
                build_spf(new_limit, &spf, &spf_limit);
            }

            /* Compute σ(next_candidate). */
            seq_t sigma = sigma_of_uint64(next_candidate, spf);

            /* Test the last‑digit condition. */
            if ((next_candidate % 10) == (sigma % 10)) {
                ++b_index;   /* b(b_index) = next_candidate */

                /* Check the ratio condition. */
                if ((next_candidate % 10) == 0 &&
                    (sigma % 10) == 0 &&
                    b_index == next_candidate / 10) {
                    cache_append(next_candidate, &seq_cache, &seq_len, &seq_cap);
                }
            }

            ++next_candidate;
        }
    }

    return seq_cache[n - 1];
}

/* --------------------------------------------------------------------- */
/* Helper: Append a value to a growing array.                            */

/* --------------------------------------------------------------------- */
/* Build smallest‑prime‑factor array up to 'limit' (inclusive).          */
/* --------------------------------------------------------------------- */
static void build_spf(idx_t limit, uint32_t **out, idx_t *limit_out)
{
    /* Allocate a fresh array of zeros. */
    uint32_t *new_spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));

    /* Classic sieve for smallest prime factor. */
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {                 /* i is prime                */
            new_spf[i] = (uint32_t)i;
            if (i <= limit / i) {              /* avoid overflow of i*i      */
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0)
                        new_spf[j] = (uint32_t)i;
            }
        }
    }

    /* Replace the old array. */
    free(*out);
    *out       = new_spf;
    *limit_out = limit;
}

/* --------------------------------------------------------------------- */
/* Compute σ(n) using the pre‑computed smallest‑prime‑factor array.    */
/* --------------------------------------------------------------------- */
static seq_t sigma_of_uint64(idx_t n, uint32_t *spf)
{
    seq_t result = 1;
    while (n > 1) {
        uint32_t p = spf[n];
        seq_t term = 1;          /* sum of p^0 .. p^a   */
        seq_t pp   = 1;          /* current power of p  */
        while (n % p == 0) {
            n /= p;
            pp *= (seq_t)p;
            term += pp;
        }
        result *= term;
    }
    return result;
}
