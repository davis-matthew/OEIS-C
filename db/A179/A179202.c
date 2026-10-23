/*
 * A179202  Numbers n such that phi(n) = phi(n+8), with Euler's totient
 *          function phi=A000010.
 * https://oeis.org/A179202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 157, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A179202: A(n) = number m such that φ(m) = φ(m+8). 
 * The function returns the n‑th such number (n ≥ 1). 
 * The implementation builds the Euler totient table incrementally, 
 * scanning for matches and caching them. 
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* --- Global state for the phi sieve and the result cache ----------------- */

static uint32_t *phi           = NULL;  /* φ values up to current limit      */
static idx_t     phi_limit     = 0;     /* highest computed index            */
static idx_t     match_last_scanned = 0;/* last m scanned for matches        */

static seq_t *match_vals        = NULL; /* dynamic array of found numbers    */
static size_t match_cap         = 0;     /* current capacity in entries       */
static size_t match_cnt         = 0;     /* number of stored terms            */

/* Grow the match array when the capacity is exceeded */
static void grow_match_vals(size_t need)
{
    if (need <= match_cap) return;
    size_t new_cap = match_cap ? match_cap * 2 : 128;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(match_vals, new_cap * sizeof(seq_t));
    match_vals = tmp;
    match_cap  = new_cap;
}

/* Store a new term into the cache */
static void add_match(idx_t v)
{
    grow_match_vals(match_cnt + 1);
    match_vals[match_cnt++] = (seq_t)v;
}

/* Rebuild the φ array up to new_limit and scan for new matches.         */
static void rebuild_phi(idx_t new_limit)
{
    if (new_limit <= phi_limit) return;

    /* Allocate a fresh φ array */
    uint32_t *new_phi = (uint32_t *)malloc((size_t)(new_limit + 1) * sizeof(uint32_t));
    if (!new_phi) return; /* allocation failed – caller will retry */

    /* Initialise with the identity function φ[i] = i */
    for (idx_t i = 0; i <= new_limit; ++i) new_phi[i] = (uint32_t)i;

    /* Classic Euler totient sieve */
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (new_phi[i] == (uint32_t)i) {           /* i is prime         */
            for (idx_t j = i; j <= new_limit; j += i)
                new_phi[j] -= new_phi[j] / (uint32_t)i;
        }
    }

    /* Scan for matches in the new interval            */
    for (idx_t m = match_last_scanned; m + 8 <= new_limit; ++m) {
        if (new_phi[m] == new_phi[m + 8]) add_match(m);
    }

    /* Replace the old φ array */
    free(phi);
    phi = new_phi;
    phi_limit = new_limit;

    /* Update the last scanned position   */
    match_last_scanned = (new_limit >= 8) ? new_limit - 8 : 0;
}

/* ------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* unused index */

    /* Ensure we have at least n terms stored */
    while (match_cnt < n) {
        idx_t next_limit = phi_limit ? phi_limit * 2ULL : 2000000ULL;
        if (next_limit <= phi_limit) next_limit = phi_limit + 1;
        rebuild_phi(next_limit);
    }

    /* Return the requested term (zero‑based index) */
    return match_vals[n - 1];
}
