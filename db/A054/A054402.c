/*
 * A054402  Numbers that are the sum of a positive square and a positive cube
 *          in more than one way.
 * https://oeis.org/A054402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_t, floor_cube_root, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054402: numbers that are the sum of a positive square and a positive cube
   in more than one way.  The implementation precomputes these numbers up to a
   dynamically increased limit by generating all sums a^2 + b^3 for 1≤a,b with
   a^2+b^3≤limit, sorting them, and extracting those that occur at least twice.
   The list of numbers is stored in ascending order and is grown only when a
   requested index exceeds the current amount of stored terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"
#include "../utils/cmp_seq_t.h"

static seq_t *terms = NULL;           /* array of found terms */
static size_t terms_len = 0;          /* number of terms stored */
static size_t terms_cap = 0;          /* allocated capacity of terms */
static idx_t  current_limit = 0;      /* current maximal sum considered */

/* --------------------------------------------------------------------- */
/* Compute all numbers a(n) up to the given integer limit (>=1).          */
/* The function allocates fresh arrays and replaces the global terms    */
/* array with the new list sorted ascending.                             */
/* --------------------------------------------------------------------- */
static void recompute_up_to(idx_t limit)
{
    seq_t limit_seq = (seq_t)limit;
    idx_t a_max = (idx_t)floor_sqrt(limit_seq);
    idx_t b_max = (idx_t)floor_cube_root(limit_seq);

    /* Count how many sums are <= limit. */
    size_t pair_cnt = 0;
    for (idx_t a = 1; a <= a_max; ++a) {
        seq_t a2 = (seq_t)a * a;
        for (idx_t b = 1; b <= b_max; ++b) {
            seq_t s = a2 + (seq_t)b * b * b;
            if (s <= limit_seq) ++pair_cnt;
        }
    }

    if (pair_cnt == 0) {
        free(terms);
        terms = NULL;
        terms_len = terms_cap = 0;
        current_limit = limit;
        return;
    }

    seq_t *sums = (seq_t *)malloc(pair_cnt * sizeof(seq_t));
    size_t idx = 0;
    for (idx_t a = 1; a <= a_max; ++a) {
        seq_t a2 = (seq_t)a * a;
        for (idx_t b = 1; b <= b_max; ++b) {
            seq_t s = a2 + (seq_t)b * b * b;
            if (s <= limit_seq) sums[idx++] = s;
        }
    }

    /* Sort all generated sums. */
    qsort(sums, pair_cnt, sizeof(seq_t), cmp_seq_t);

    /* Extract values that occur at least twice. */
    seq_t *new_terms = NULL;
    size_t new_len = 0, new_cap = 0;
    idx = 0;
    while (idx < pair_cnt) {
        idx_t j = idx + 1;
        while (j < pair_cnt && sums[j] == sums[idx]) ++j;
        if ((size_t)(j - idx) >= 2) {
            if (new_len == new_cap) {
                new_cap = new_cap ? new_cap * 2 : 4;
                new_terms = (seq_t *)realloc(new_terms, new_cap * sizeof(seq_t));
            }
            new_terms[new_len++] = sums[idx];
        }
        idx = j;
    }

    free(sums);
    free(terms);
    terms = new_terms;
    terms_len = new_len;
    terms_cap = new_cap;
    current_limit = limit;
}

/* --------------------------------------------------------------------- */
/* Ensure that at least 'n' terms have been computed.                    */
/* If not, double the current limit until enough terms are found.       */
/* --------------------------------------------------------------------- */
static void ensure_terms(idx_t n)
{
    if (terms_len >= (size_t)n) return;
    /* start with a reasonable initial limit if not yet set */
    if (current_limit == 0) current_limit = 5000000;
    while (terms_len < (size_t)n) {
        current_limit *= 2;
        recompute_up_to(current_limit);
    }
}

/* --------------------------------------------------------------------- */
/* Public API required by the build.                                    */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The sequence is 1‑based; n>=1. */
    if (n == 0) return 0;          /* undefined, but avoid segfault */
    ensure_terms(n);
    return terms[n - 1];
}
