/*
 * A100562  Write down the numbers from 7 to infinity. Take next number, M
 *          say, that has not been crossed off and cross off all the numbers
 *          i*M - 1 for i >= 2. Repeat. The numbers that are left form the
 *          sequence.
 * https://oeis.org/A100562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A100562:
   For each new survivor M (starting from 7),
   cross out all numbers of the form i*M‑1 for i ≥ 2.
   The numbers that remain form the sequence.
   This implementation uses an incremental sieve with dynamic arrays.
   The algorithm is O(n log n) and works for arbitrary order of calls. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static state */
static idx_t seq_len   = 0;
static idx_t seq_cap   = 64;
static seq_t *seq_arr  = NULL;

static idx_t surv_len  = 0;
static idx_t surv_cap  = 64;
static seq_t *surv_arr = NULL;

static idx_t cross_cap = 0;
static unsigned char *cross_arr = NULL;

static idx_t next_candidate = 7;          /* next number to test for survival */

/* Extend the cross array so that it can hold indices up to at least new_cap.
   Existing crossings are preserved; crossings for previously found survivors
   are applied to the newly allocated part. */
static void extend_cross(idx_t new_cap)
{
    unsigned char *new_cross = (unsigned char*)calloc(new_cap, sizeof *new_cross);
    if (!cross_arr) {                     /* first allocation */
        cross_arr = new_cross;
        cross_cap = new_cap;
        return;
    }
    /* Copy old data and free old buffer */
    memcpy(new_cross, cross_arr, cross_cap);
    free(cross_arr);
    cross_arr = new_cross;
    idx_t old_cap = cross_cap;
    cross_cap = new_cap;

    /* Mark new region for each survivor */
    for (idx_t s = 0; s < surv_len; ++s) {
        seq_t m = surv_arr[s];
        /* Convert to idx_t if it fits; if it does not, it will not affect this pass */
        idx_t m_idx = (idx_t)m;
        if (m_idx == 0) continue;
        /* Find first j such that j*m - 1 >= old_cap */
        idx_t j_start = (old_cap + m_idx) / m_idx;
        if (j_start < 2) j_start = 2;
        /* Find last j such that j*m - 1 < new_cap */
        idx_t j_end = (new_cap - 1) / m_idx;
        for (idx_t j = j_start; j <= j_end; ++j) {
            idx_t t = j * m_idx - 1;
            if (t < new_cap) cross_arr[t] = 1;
        }
    }
}

/* Mark crossings for a new survivor m up to the current limit of the
   cross array. */
static void mark_to_cap(seq_t m, idx_t limit)
{
    if (!cross_arr) return;
    idx_t m_idx = (idx_t)m;
    if (m_idx == 0) return;
    idx_t max_j = limit / m_idx;
    for (idx_t j = 2; j <= max_j; ++j) {
        idx_t t = j * m_idx - 1;
        if (t < limit) cross_arr[t] = 1;
    }
}

/* Grow seq_arr or surv_arr if necessary. */
static void maybe_grow_seq(void)
{
    if (seq_len >= seq_cap) {
        seq_cap *= 2;
        seq_arr = (seq_t*)realloc(seq_arr, seq_cap * sizeof(seq_t));
    }
}
static void maybe_grow_surv(void)
{
    if (surv_len >= surv_cap) {
        surv_cap *= 2;
        surv_arr = (seq_t*)realloc(surv_arr, surv_cap * sizeof(seq_t));
    }
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, but harmless */

    /* one‑time initialization */
    if (!seq_arr) {
        seq_arr   = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        surv_arr  = (seq_t*)malloc(surv_cap * sizeof(seq_t));
        extend_cross(2048);               /* small initial sieve */
    }

    /* Generate terms until we have at least the n‑th one. */
    while (seq_len < n) {
        /* Ensure we have space to inspect next_candidate. */
        if (!cross_arr || next_candidate >= cross_cap) {
            extend_cross(next_candidate + 1024);   /* grow beyond needed point */
        }

        /* Skip numbers that have been crossed out. */
        while (next_candidate < cross_cap && cross_arr[next_candidate]) {
            ++next_candidate;
            if (next_candidate >= cross_cap) extend_cross(next_candidate + 1024);
        }

        /* next_candidate is a new survivor. */
        maybe_grow_seq();
        seq_arr[seq_len++] = (seq_t)next_candidate;

        maybe_grow_surv();
        surv_arr[surv_len++] = (seq_t)next_candidate;

        /* Cross numbers of the form j*next_candidate - 1 up to current limit. */
        mark_to_cap((seq_t)next_candidate, cross_cap);

        ++next_candidate;
    }

    return seq_arr[n - 1];
}
