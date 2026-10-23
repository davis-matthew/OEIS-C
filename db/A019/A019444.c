/*
 * A019444  a_1, a_2, ..., is a permutation of the positive integers such that
 *          the average of each initial segment is an integer, using the
 *          greedy algorithm to define a_n.
 * https://oeis.org/A019444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A002251(n-1) + 1 Recurrence (greedy algorithm): let s =
 *            average of first k terms if s not yet used as a value, a(k+1) =
 *            s, s stays the same else a(k+1) = s + k + 1, s = s + 1 Sequence
 *            starts with a(1)=1
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static size_t max_computed = 0;   /* highest n that has been computed */
static seq_t *a_vals      = NULL; /* a[1..]  (1‑indexed) */
static size_t a_cap       = 0;    /* allocated capacity of a_vals   */
static seq_t current_s;           /* average after max_computed terms */

static bool *used_vals     = NULL; /* used[0..]  guard of used values   */
static size_t used_cap     = 0;    /* allocated capacity of used_vals */

/* Ensure that a_vals can hold indices up to n (inclusive). */
static void ensure_a_cap(idx_t n)
{
    size_t new_cap = (size_t)n + 1; /* 1‑indexed */
    if (new_cap > a_cap) {
        seq_t *tmp = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_vals = tmp;
        a_cap  = new_cap;
    }
}

/* Ensure that used_vals can index up to at least min_idx. */
static void ensure_used_cap(size_t min_idx)
{
    if (min_idx < used_cap) return;
    size_t old = used_cap;
    size_t new_cap = (used_cap == 0) ? 8 : used_cap;
    while (new_cap <= min_idx) new_cap <<= 1;
    bool *tmp = (bool*)realloc(used_vals, new_cap * sizeof(bool));
    if (!tmp) abort();
    for (size_t i = old; i < new_cap; ++i) tmp[i] = false;
    used_vals = tmp;
    used_cap  = new_cap;
}

/* Main routine. */
seq_t a(idx_t n)
{
    if (n == 0) abort(); /* sequence is 1‑indexed */

    /* initialisation on first call */
    if (max_computed == 0) {
        ensure_a_cap(1);
        ensure_used_cap(1);
        a_vals[1] = (seq_t)1;
        used_vals[(size_t)1] = true;
        current_s = (seq_t)1;
        max_computed = 1;
    }

    /* If already computed, just return it. */
    if ((size_t)n <= max_computed) return a_vals[(size_t)n];

    /* Build sequence up to n. */
    for (idx_t cur = max_computed; cur < n; ++cur) {
        idx_t next = cur + 1;

        /* Make sure arrays can hold the new term. */
        ensure_a_cap(next);

        /* Ensure used array can index current_s and possible candidate. */
        size_t cs_idx = (size_t)current_s;
        if (cs_idx >= used_cap) ensure_used_cap(cs_idx);

        seq_t cand;
        if (used_vals[cs_idx]) { /* current average already used */
            cand = current_s + (seq_t)next;
            current_s = current_s + (seq_t)1;
        } else {
            cand = current_s;
            /* current_s unchanged */
        }

        /* Ensure used array can track cand. */
        size_t cand_idx = (size_t)cand;
        if (cand_idx >= used_cap) ensure_used_cap(cand_idx);

        a_vals[next]      = cand;
        used_vals[cand_idx] = true;
    }

    max_computed = (size_t)n;
    return a_vals[(size_t)n];
}
