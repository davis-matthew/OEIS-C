/*
 * A022952  a(n) = a(n-1) + c(n) for n >= 3, a( ) increasing, given a(1)=3
 *          a(2)=6; where c( ) is complement of a( ).
 * https://oeis.org/A022952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + c(n) with a(1)=3, a(2)=6 * c(n) is the nth
 *            positive integer that is NOT a member of the sequence a. * The
 *            code builds the sequence incrementally and memoises already *
 *            computed terms, enabling random‑order lookups
 * Range:     u8 n <= 19, u16 n <= 346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for the computed sequence */
static seq_t *a_vals = NULL;    /* array of terms (1‑indexed in OEIS sense)   */
static size_t a_len  = 0;        /* number of terms currently stored          */
static size_t a_cap  = 0;        /* allocated capacity of a_vals              */

/* state used while searching for the next complement */
static seq_t  next_comp = (seq_t)1; /* integer currently examined as a potential complement */
static idx_t  comp_idx  = 0;        /* index into a_vals of the first element
                                      that is NOT less than next_comp             */
static idx_t  non_a_cnt = 0;        /* how many complement numbers have been counted */

/* ------------------------------------------------------------------ */
/* Helper to grow the sequence array when the capacity is exhausted. */
static void ensure_a_capacity(void)
{
    if (a_len >= a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        a_vals = tmp;
        a_cap  = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Initialise the sequence with the two given terms. */
static void init_sequence(void)
{
    ensure_a_capacity();          /* guarantees at least 2 slots          */
    a_vals[0] = (seq_t)3;         /* a(1) */
    a_vals[1] = (seq_t)6;         /* a(2) */
    a_len   = 2;
    next_comp = (seq_t)1;         /* first candidate for the complement   */
    comp_idx  = 0;
    non_a_cnt = 0;
}

/* ------------------------------------------------------------------ */
/* The required function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* indices below 1 are never requested */

    if (a_len == 0) init_sequence();

    if (n <= a_len) return a_vals[n-1];

    /* generate terms up to n, using the recursive rule and the
       definition of the complement sequence. */
    for (idx_t cur = a_len + 1; cur <= n; ++cur) {
        /* find the cur‑th complement number */
        while (non_a_cnt < cur) {
            /* skip a_vals that are less than the current candidate */
            while (comp_idx < a_len && a_vals[comp_idx] < next_comp)
                ++comp_idx;

            /* if the candidate is an existing term, skip it */
            if (comp_idx < a_len && a_vals[comp_idx] == next_comp) {
                ++next_comp;            /* move to the next integer   */
                continue;
            }

            /* otherwise the candidate is a non‑term (a complement)   */
            ++non_a_cnt;
            if (non_a_cnt == cur) {
                /* this is the required complement for the current term   */
                seq_t val = a_vals[a_len - 1] + next_comp;
                ensure_a_capacity();
                a_vals[a_len++] = val;        /* append the new term   */
                ++next_comp;                  /* advance for future    */
                break;                         /* finished generating a[cur] */
            } else {
                /* not yet reached the cur‑th complement – just count it */
                ++next_comp;
            }
        }
    }

    return a_vals[n-1];
}
