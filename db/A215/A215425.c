/*
 * A215425  Primes congruent to {0, 1, 2, 4, 5, 6} mod 17.
 * https://oeis.org/A215425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime congruent to 0,1,2,4,5,6 mod 17
   (plus the special prime 2). It builds a dynamic list of all primes
   found so far and keeps the ones that satisfy the residue condition
   in a separate list.  It works for arbitrary‑precision seq_t by
   casting the selected primes to seq_t on return. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t *all_primes = NULL;     /* all primes found so far    */
static size_t   full_size  = 0;
static size_t   full_cap   = 0;

static uint64_t *selected = NULL;       /* primes that satisfy the residue condition   */
static size_t   sel_size  = 0;
static size_t   sel_cap   = 0;

/* current number to test (next_candidate is the next value after the
   one currently being checked) */
static uint64_t next_candidate = 2;

/* Grow an array of uint64_t to at least 'min_cap' elements. */
static void ensure_full_capacity(size_t min_cap) {
    if (full_cap >= min_cap) return;
    size_t new_cap = full_cap ? full_cap * 2 : 4;
    while (new_cap < min_cap) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(all_primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    all_primes = tmp;
    full_cap   = new_cap;
}

/* Grow an array of uint64_t to at least 'min_cap' elements (for selected). */
static void ensure_sel_capacity(size_t min_cap) {
    if (sel_cap >= min_cap) return;
    size_t new_cap = sel_cap ? sel_cap * 2 : 4;
    while (new_cap < min_cap) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(selected, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    selected = tmp;
    sel_cap   = new_cap;
}

/* Test whether 'cand' is prime using the primes found so far. */
static bool is_prime_candidate(uint64_t cand) {
    if (cand == 2) return true;
    if ((cand & 1ULL) == 0ULL) return false;        /* even numbers > 2 are composite */

    for (size_t i = 0; i < full_size; ++i) {
        uint64_t p = all_primes[i];
        if (p > cand / p) break;                   /* p^2 > cand */
        if (cand % p == 0ULL) return false;
    }
    return true;
}

/* --- sequence function --- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;                    /* behaviour below offset is irrelevant */

    /* Generate primes until we have at least n selected primes */
    while (sel_size < n) {
        uint64_t cand = next_candidate;

        /* update candidate for next iteration */
        next_candidate = (cand == 2ULL) ? 3ULL : cand + 2ULL;   /* skip even numbers after 2 */

        if (is_prime_candidate(cand)) {
            /* store the prime in the full list */
            ensure_full_capacity(full_size + 1);
            all_primes[full_size++] = cand;

            /* check residue condition */
            uint64_t r = cand % 17ULL;
            if (r == 0ULL || r == 1ULL || r == 2ULL || r == 4ULL || r == 5ULL || r == 6ULL) {
                ensure_sel_capacity(sel_size + 1);
                selected[sel_size++] = cand;
            }
        }
    }

    return (seq_t)selected[n - 1];
}
