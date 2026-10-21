/*
 * A209746  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209745; see the Formula section.
 * https://oeis.org/A209746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  T(1,0)=1,  T(2,0)=T(2,1)=2
 *          T(n,k)=T(n-1,k)+2·T(n-1,k-1)+T(n-2,k)+T(n-2,k-1)
 *          (missing entries are treated as 0).
 * The sequence a(n) is the row‑wise flattening of T; a(1)=1,
 * a(2)=2, a(3)=2, a(4)=3, a(5)=7, a(6)=4, ….
 */

#include <stdlib.h>

/*  Static storage for the flattened sequence. */
static seq_t *flat = NULL;      /* read‑only sequence values */
static size_t flat_len = 0;     /* number of values currently stored */
static size_t flat_cap = 0;     /* allocated capacity of flat */

/*  Storage for the two previous rows in the triangle. */
static seq_t *row_prev    = NULL;    /* row n‑1   */
static size_t row_prev_len = 0;
static seq_t *row_prevprev = NULL;   /* row n‑2   */
static size_t row_prevprev_len = 0;

/*  Ensure the flattened array has at least new_cap elements. */
static void grow_flat(size_t new_cap) {
    if (new_cap <= flat_cap) return;
    size_t cap = (flat_cap == 0) ? 16 : flat_cap;
    while (cap < new_cap) cap <<= 1;
    flat = (seq_t *)realloc(flat, cap * sizeof(seq_t));
    flat_cap = cap;
}

/*  Ensure that the flattened array contains at least n terms. */
static void ensure_terms(idx_t n) {
    if (flat_len >= (size_t)n) return;

    /*  Initialise first two rows if this is the first request. */
    if (flat_len == 0) {
        /* Row 1: T(1,0)=1 */
        row_prevprev_len = 1;
        row_prevprev = (seq_t *)malloc(1 * sizeof(seq_t));
        row_prevprev[0] = (seq_t)1;
        grow_flat(1);
        flat[flat_len++] = row_prevprev[0];
        row_prevprev_len = 1;

        /* Row 2: T(2,0)=T(2,1)=2 */
        row_prev_len = 2;
        row_prev = (seq_t *)malloc(2 * sizeof(seq_t));
        row_prev[0] = (seq_t)2;
        row_prev[1] = (seq_t)2;
        grow_flat(3);
        flat[flat_len++] = row_prev[0];
        flat[flat_len++] = row_prev[1];
    }

    /*  Generate rows until we have at least n terms. */
    while (flat_len < (size_t)n) {
        size_t new_len = row_prev_len + 1;            /* row length */
        seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));

        for (size_t k = 0; k < new_len; ++k) {
            seq_t sum = (seq_t)0;
            /*  T(n-1,k) */
            if (k < row_prev_len) sum += row_prev[k];
            /*  2·T(n-1,k-1) */
            if (k >= 1 && k-1 < row_prev_len) sum += (seq_t)2 * row_prev[k-1];
            /*  T(n-2,k) */
            if (k < row_prevprev_len) sum += row_prevprev[k];
            /*  T(n-2,k-1) */
            if (k >= 1 && k-1 < row_prevprev_len) sum += row_prevprev[k-1];
            new_row[k] = sum;
        }

        /* Append new row to the flattened array. */
        grow_flat(flat_len + new_len);
        for (size_t k = 0; k < new_len; ++k) {
            flat[flat_len++] = new_row[k];
        }

        /*  Advance the row pointers. */
        free(row_prevprev);
        row_prevprev = row_prev;
        row_prevprev_len = row_prev_len;

        row_prev = new_row;
        row_prev_len = new_len;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* out of bounds, harmless */
    ensure_terms(n);
    return flat[(size_t)(n - 1)];
}
