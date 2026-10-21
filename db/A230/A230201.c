/*
 * A230201  Numbers k such that sigma(phi(k)) < k.
 * https://oeis.org/A230201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 3994, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A230201: numbers k such that sigma(phi(k)) < k,
   by incremental search. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/sigma_of.h"

/* ------------------------------------------------------------------ */
/*  Sequence state                                               */
static seq_t *terms_tbl = NULL;   /* array of found terms (seq_t)   */
static size_t terms_len   = 0;     /* number of terms stored        */
static size_t terms_cap   = 0;     /* capacity of terms_tbl          */
static idx_t   upper_limit = 1;    /* largest k that has been examined */

/* ------------------------------------------------------------------ */
/*  Grow the terms array if needed                                */
static void ensure_capacity(size_t needed)
{
    if (needed <= terms_cap)
        return;
    size_t new_cap = terms_cap == 0 ? 256 : terms_cap;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_tbl = (seq_t *)realloc(terms_tbl, new_cap * sizeof(seq_t));
    if (!new_tbl) {
        /* Allocation failure – abort (should not happen in tests) */
        abort();
    }
    terms_tbl = new_tbl;
    terms_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Generate terms until the array holds at least n entries           */
static void ensure_terms_up_to(idx_t n)
{
    if (n <= (idx_t)terms_len)
        return;
    ensure_capacity((size_t)n);

    while ((idx_t)terms_len < n) {
        ++upper_limit;                            /* next k to test   */
        seq_t k_seq = (seq_t)upper_limit;          /* k as seq_t       */
        seq_t phi_k = calc_phi(k_seq);            /* φ(k)             */
        seq_t sigma_phi = sigma_of((idx_t)phi_k); /* σ(φ(k))          */
        if (sigma_phi < k_seq)                    /* condition?       */
            terms_tbl[terms_len++] = k_seq;        /* store the term   */
    }
}

/* ------------------------------------------------------------------ */
/*  Main API: a(n) – nth term (1‑based) of the sequence                */
seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0; /* undefined for n < 1 */
    }
    ensure_terms_up_to(n);
    return terms_tbl[n - 1];
}
