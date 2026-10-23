/*
 * A024804  Numbers that are the sum of 3 distinct nonzero squares in 2 or
 *          more ways.
 * https://oeis.org/A024804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: numbers that can be expressed as the sum of three
 * distinct non‑zero integer squares in at least two distinct ways.
 * The algorithm enumerates all triples a<b<c with a^2+b^2+c^2
 * up to a current search bound, counts the representations,
 * and collects the numbers that occur at least twice. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* static state for the memoised sequence */
static uint16_t *counts   = NULL;   /* number of representations for each sum */
static size_t   counts_cap = 0;     /* size of `counts` in entries          */

static seq_t   *terms     = NULL;   /* sorted list of valid terms            */
static size_t   terms_cap = 0;       /* allocated size of `terms`              */
static size_t   terms_sz  = 0;       /* current number of terms in `terms`     */

/* ensure that `counts` has capacity for at least `cap` entries */
static void ensure_counts_cap(size_t cap)
{
    if (cap > counts_cap) {
        uint16_t *tmp = (uint16_t *)realloc(counts, cap * sizeof(uint16_t));
        if (tmp == NULL) exit(1);                  /* allocation failure */
        /* Zero the new region */
        memset(tmp + counts_cap, 0, (cap - counts_cap) * sizeof(uint16_t));
        counts     = tmp;
        counts_cap = cap;
    }
}

/* recompute all counts up to `maxx` (inclusive) */
static void recompute_counts(idx_t maxx)
{
    size_t cap = (size_t)(maxx + 1);
    ensure_counts_cap(cap);
    /* Zero all entries up to maxx */
    memset(counts, 0, cap * sizeof(uint16_t));

    /* Enumerate triples a<b<c */
    for (uint64_t a = 1; a*a < (uint64_t)maxx; ++a) {
        __uint128_t a2 = (__uint128_t)a * a;
        for (uint64_t b = a + 1; b*b < (uint64_t)maxx - (uint64_t)a2; ++b) {
            __uint128_t b2 = (__uint128_t)b * b;
            __uint128_t ab = a2 + b2;
            for (uint64_t c = b + 1; ; ++c) {
                __uint128_t c2 = (__uint128_t)c * c;
                __uint128_t s  = ab + c2;
                if (s > (uint64_t)maxx) break;
                ++counts[(size_t)s];
            }
        }
    }
}

/* rebuild the list of terms from the current `counts` array up to `maxx` */
static void rebuild_terms(idx_t maxx)
{
    /* Allocate space for at most one term per integer <= maxx */
    if (terms_cap < (size_t)(maxx + 1)) {
        seq_t *tmp = (seq_t *)realloc(terms, (maxx + 1) * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        terms     = tmp;
        terms_cap = (size_t)(maxx + 1);
    }

    terms_sz = 0;
    for (idx_t x = 1; x <= maxx; ++x) {
        if (counts[(size_t)x] >= 2) {
            terms[terms_sz++] = (seq_t)x;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of bounds, avoid UB */

    /* If we already have the requested term, return it */
    if (terms_sz >= (size_t)n) return terms[n - 1];

    /* Start with a small search bound and grow until we have enough terms */
    idx_t search_max = 200;                /* initial upper bound */
    while (terms_sz < (size_t)n) {
        recompute_counts(search_max);
        rebuild_terms(search_max);
        if (terms_sz >= (size_t)n) break;
        search_max *= 2;                   /* double the search range for next pass */
    }

    return terms[n - 1];
}
