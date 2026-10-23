/*
 * A020488  Numbers n such that tau(n) (or sigma_0(n)) = phi(n).
 * https://oeis.org/A020488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A020488 – numbers n with tau(n) = phi(n).
   The function returns the n‑th such number (a(1) = 1). */

#include <stdlib.h>
#include <stdbool.h>
/* helper that builds an array of smallest prime factors (spf[0] unused);
   spf[i] = 0 or the smallest prime dividing i (for primes spf[i] = 0). */
#include "../utils/build_spf.h"

/* static state for memoisation */
static seq_t *cache = NULL;          /* array of found terms          */
static size_t cache_len = 0;         /* number of terms stored        */
static size_t cache_cap = 0;         /* allocated capacity            */

static idx_t *spf = NULL;            /* smallest prime factor array   */
static idx_t spf_limit = 0;          /* maximum n for which spf is ready */

static idx_t next_test = 1;          /* next integer to test          */

/* Ensure spf array contains 0..limit */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;
    /* free previous array */
    if (spf != NULL)
        free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* Append a new term to the cache, expanding it if needed */
static void push_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* abort on OOM          */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Find the n‑th term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* out of bounds          */

    /* Compute terms until we have at least n of them */
    while (cache_len < n) {
        /* Make sure the spf array covers the current candidate */
        ensure_spf(next_test);

        idx_t m = next_test;
        seq_t phi_val = (seq_t)m;
        seq_t tau_val = (seq_t)1;
        idx_t tmp = m;

        /* Factorise m with the pre‑computed spf array */
        while (tmp > 1) {
            idx_t p = spf[tmp];
            if (p == 0)               /* prime when spf not set */
                p = tmp;
            int cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            phi_val -= phi_val / p;
            tau_val *= (seq_t)(cnt + 1);
        }

        if (phi_val == tau_val)
            push_term((seq_t)m);

        ++next_test;
    }

    /* Return the stored n‑th term */
    return cache[n - 1];
}
