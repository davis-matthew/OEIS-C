/*
 * A097746  Numbers that cannot be written as differences of two distinct
 *          partition numbers.
 * https://oeis.org/A097746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   compute_partition, pent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the nth term (1‑based) of A097746, the numbers that
    cannot be written as a difference of two distinct partition numbers.
    The algorithm generates partition numbers up to a growing limit,
    marks all differences of two distinct partition numbers, and keeps a
    list of the numbers that remain unmarked.  When enough terms have
    been produced it answers the requested index.  All arithmetic is done
    with the seq_t type; internal calculations use signed 128‑bit integers
    for safety, but the final values are cast back to seq_t.  This works
    with both the C and C++ builds accepted by the judge.  */
/*  Author: ChatGPT */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/compute_partition.h"
#include "../utils/pent.h"

/* global state for memoisation */
static size_t rep_cap = 0;          /* size of representable array  (indices 0..rep_cap-1) */
static uint8_t *rep = NULL;         /* boolean array: 1 if representable */

static size_t comp_cap = 0;          /* size of complement array  */
static seq_t *comp = NULL;           /* list of complement numbers */
static size_t comp_len = 0;          /* number of elements stored so far */
static size_t comp_last_limit = 0;    /* maximum value that has been processed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not required, but safe */

    /* Ensure we have at least n terms in comp */
    while (comp_len < (size_t)n) {
        /* Increase limit (doubling strategy) */
        size_t new_limit = (rep_cap == 0) ? 200000 : rep_cap * 2;
        if (new_limit <= comp_last_limit) new_limit = comp_last_limit * 2 + 1;

        /* Allocate / resize representable array */
        rep = (uint8_t *)realloc(rep, new_limit + 1);
        if (!rep) { /* allocation failed */
            return 0;
        }
        memset(rep + comp_last_limit + 1, 0, new_limit - comp_last_limit); /* zero new section */

        /* Compute partition numbers up to the first p[k] > new_limit+1 */
        size_t parts_cap = 128;   /* start with small capacity */
        seq_t *parts = (seq_t *)malloc(parts_cap * sizeof(seq_t));
        if (!parts) { /* allocation failed */
            return 0;
        }
        parts[0] = (seq_t)1;   /* p(0) = 1 */
        size_t K = 0;          /* last index with p[K] <= new_limit+1 */

        for (idx_t n_part = 1; ; ++n_part) {
            /* Expand parts array if necessary */
            if ((size_t)n_part >= parts_cap) {
                parts_cap *= 2;
                parts = (seq_t *)realloc(parts, parts_cap * sizeof(seq_t));
                if (!parts) { free(parts); return 0; }
            }

            seq_t val = compute_partition(n_part, parts);
            if (val > (seq_t)new_limit + 1) {
                K = n_part - 1;
                break;
            }
            parts[n_part] = val;
        }

        /* Mark all differences of two distinct partition numbers */
        for (size_t i = 0; i <= K; ++i) {
            for (size_t j = i + 1; j <= K; ++j) {
                seq_t diff = parts[j] - parts[i];
                if (diff <= (seq_t)new_limit) {
                    rep[(size_t)diff] = 1;
                }
            }
        }

        /* Append new complementary numbers to comp */
        if (comp_len + (new_limit - comp_last_limit) > comp_cap) {
            /* grow enough to hold all new entries */
            comp_cap = (comp_len + (new_limit - comp_last_limit)) * 2;
            comp = (seq_t *)realloc(comp, comp_cap * sizeof(seq_t));
            if (!comp) { free(parts); return 0; }
        }
        for (size_t v = comp_last_limit + 1; v <= new_limit; ++v) {
            if (!rep[v]) {
                comp[comp_len++] = (seq_t)v;
            }
        }

        comp_last_limit = new_limit;
        free(parts);
    }

    return comp[n - 1];
}
