/*
 * A096925  Numbers n for which there are exactly four k such that n = k +
 *          (product of nonzero digits of k).
 * https://oeis.org/A096925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A096925 – Numbers n for which there are exactly four k such that
 *     n = k + product of nonzero decimal digits of k.
 * The algorithm builds a sieve over k, recording for each possible n
 * how many k map to it.  The set of n with count = 4 is kept in a
 * sorted list and returned by index. */

/* ------------------------------------------------------------ */
#include <stdlib.h>
#include <stdint.h>
void abort(void);                     /* declaration used for abort() */

/* ------------------------------------------------------------ */
/* Helper: product of non‑zero decimal digits of x                     */
static seq_t prod_nonzero(idx_t x)
{
    seq_t p = 1;
    while (x != 0) {
        int d = (int)(x % 10);
        if (d != 0) {
            unsigned __int128 tmp = (unsigned __int128)p * d;
            p = (seq_t)tmp;
        }
        x /= 10;
    }
    return p;
}

/* ------------------------------------------------------------ */
/* Dynamically sized integer array for counts                     */
static unsigned *counts     = NULL;   /* counts[n]  */
static size_t   counts_cap  = 0;
static idx_t    cur_limit   = 0;     /* largest k processed */

/* ------------------------------------------------------------ */
/* Dynamically sized seq_t array for terms (n with count==4)       */
static seq_t  *terms      = NULL;
static size_t  terms_size = 0;
static size_t  terms_cap  = 0;

/* ------------------------------------------------------------ */
/* Ensure the counts array can hold indices up to new_cap‑1.         */
static void ensure_counts_capacity(size_t new_cap)
{
    if (new_cap <= counts_cap) return;
    size_t cap = counts_cap ? counts_cap : 1;
    while (cap < new_cap) cap <<= 1;          /* exponential grow */
    unsigned *tmp = (unsigned *)realloc(counts, cap * sizeof(unsigned));
    if (!tmp) abort();
    /* initialise new part to zero */
    for (size_t i = counts_cap; i < cap; ++i) tmp[i] = 0;
    counts    = tmp;
    counts_cap = cap;
}

/* ------------------------------------------------------------ */
/* Ensure the terms array can hold at least new_cap elements.      */
static void ensure_terms_capacity(size_t new_cap)
{
    if (new_cap <= terms_cap) return;
    size_t cap = terms_cap ? terms_cap : 1;
    while (cap < new_cap) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
    if (!tmp) abort();
    terms    = tmp;
    terms_cap = cap;
}

/* ------------------------------------------------------------ */
/* Rebuild the sorted list of n where counts[n] == 4.              */
static void rebuild_terms(idx_t up_to)
{
    terms_size = 0;
    for (idx_t n = 1; n <= up_to; ++n) {
        if (counts[(size_t)n] == 4) {
            if (terms_size == terms_cap) {
                ensure_terms_capacity(terms_size + 1);
            }
            terms[terms_size++] = (seq_t)n;
        }
    }
}

/* ------------------------------------------------------------ */
/* Expand the sieve up to new_limit (new_limit > cur_limit).     */
static void expand_to(idx_t new_limit)
{
    if (new_limit <= cur_limit) return;
    ensure_counts_capacity((size_t)new_limit + 1);
    for (idx_t k = cur_limit + 1; k <= new_limit; ++k) {
        seq_t prod = prod_nonzero(k);
        seq_t n    = (seq_t)k + prod;
        if (n <= (seq_t)new_limit) {
            counts[(size_t)n]++;        /* one more k maps to n */
        }
    }
    cur_limit = new_limit;
    rebuild_terms(new_limit);
}

/* ------------------------------------------------------------ */
/* Make sure we have at least req terms available.               */
static void ensure_terms(idx_t req)
{
    if (req <= terms_size) return;
    /* Heuristic: start with a moderate size and double until enough */
    idx_t limit = 20000;                 /* initial guess */
    if (limit < req * 2) limit = req * 2;
    while (terms_size < (size_t)req) {
        expand_to(limit);
        limit <<= 1;                     /* double the bound each round */
        if (limit < 1000) limit = 1000;  /* safety for very small req */
    }
}

/* ------------------------------------------------------------ */
/* Public API                                                            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for index 0 */
    ensure_terms(n);
    return terms[(size_t)n - 1];
}
