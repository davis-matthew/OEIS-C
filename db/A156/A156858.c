/*
 * A156858  Composite numbers k such that sum of prime factors of k + number
 *          of prime factors of k = prime.
 * https://oeis.org/A156858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   build_spf, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A156858: composite numbers k with sum(p_i)+ω(k) prime
   (ω = number of prime factors counted with multiplicity).
   The function builds a dynamic SPF (smallest prime factor) table
   and caches computed sequence terms.  It uses the helper
   functions build_spf and is_prime from the OEIS build harness. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/build_spf.h"
#include "../utils/is_prime.h"

/* static state ----------------------------------------------------- */
static idx_t *spf          = NULL;   /* SPF table up to spf_limit */
static idx_t  spf_limit    = 0;      /* current maximum of spf */
static seq_t *results      = NULL;   /* cached sequence terms */
static size_t results_len  = 0;      /* number of terms stored */
static size_t results_cap  = 0;      /* capacity of results array */
static idx_t  checked_upto = 3;      /* smallest number we have examined */

/* Ensure the SPF table covers at least 'limit'. */
static void recompute_spf(idx_t limit) {
    if (limit <= spf_limit) return;
    free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* Append a new term to the static results array. */
static void append_result(seq_t val) {
    if (results_len == results_cap) {
        size_t new_cap = results_cap ? results_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* avoid OOM */
        results = tmp;
        results_cap = new_cap;
    }
    results[results_len++] = val;
}

/* Compute at least the first 'n' terms of the sequence. */
static void ensure_n_terms(idx_t n) {
    while (results_len < n) {
        ++checked_upto;
        idx_t k = checked_upto;

        /* Expand the SPF table if necessary. */
        if (k > spf_limit)
            recompute_spf(k * 2 + 10);

        /* Skip 1–3 and primes. */
        if (k < 4 || spf[k] == k) continue;

        /* Factor k using the SPF table. */
        seq_t sum_pf = 0;
        seq_t omega  = 0;
        idx_t temp   = k;
        while (temp > 1) {
            idx_t p = spf[temp];
            sum_pf += (seq_t)p;
            omega++;
            temp /= p;
        }

        /* Test the condition. */
        seq_t cand = sum_pf + omega;
        /* cand is small (fits in idx_t for the range of n we handle). */
        if (is_prime((idx_t)cand))
            append_result((seq_t)k);
    }
}

/* Public entry point ------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset is 1; a(0) undefined */
    ensure_n_terms(n);
    return results[n - 1];
}
