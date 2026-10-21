/*
 * A062704  Di-Boustrophedon transform of all 1's sequence: Fill in an array
 *          by diagonals alternating in the 'up' and 'down' directions. Each
 *          diagonal starts with a 1. When going in the 'up' direction the
 *          next element is the sum of the previous element of the diagonal
 *          and the previous two elements of the row...
 * https://oeis.org/A062704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A062704 – Di‑Boustrophedon transform of all 1's sequence
 *
 *  The code builds the array row by row, filling the diagonals
 *  alternately up and down.  Each diagonal starts with 1.
 *
 *  For an up‑diagonal the next element = previous diagonal element
 *  + previous two elements of its row.
 *
 *  For a down‑diagonal the next element = previous diagonal element
 *  + previous two elements of its column.
 *
 *  The last element of the n‑th diagonal is a(n).  The function
 *  returns this value for any n ≥ 1.
 */

#ifndef A062704_H
#define A062704_H

#include <stdlib.h>
#include <stdbool.h>

/* Define the function to compute the sequence.  The types seq_t
 * and idx_t are supplied by the build system.  seq_t may be an
 * arbitrary‑precision integer type when compiled as C++17; in
 * that case this file uses std::vector to guarantee correct
 * object copying.  In a plain C build seq_t is a plain integer
 * type and a simple malloc/realloc strategy suffices. */
#ifdef __cplusplus
#include <vector>
#endif

static idx_t maxComputed = 0;            /* highest diagonal computed */
static idx_t allocSize   = 0;            /* current allocation size (n+1) */

#ifndef __cplusplus
/* Plain C implementation – use raw pointers and realloc. */
static seq_t *a_cache   = NULL;
static seq_t *row_prev1 = NULL;
static seq_t *row_prev2 = NULL;
static seq_t *col_prev1 = NULL;
static seq_t *col_prev2 = NULL;

/* Ensure arrays are large enough for indices up to n.  All new
 * entries are zeroed so that missing row or column values are
 * treated as 0. */
static void ensure_capacity(idx_t n)
{
    if (allocSize >= (size_t)n + 1) return;

    size_t newSize = (size_t)n + 1;

    /* Reallocate each array, expanding it to newSize.  The
     * new elements are initialized to 0 by calloc. */
    if (a_cache == NULL)
        a_cache = (seq_t *)calloc(newSize, sizeof(seq_t));
    else {
        seq_t *tmp = (seq_t *)realloc(a_cache, newSize * sizeof(seq_t));
        if (tmp == NULL) abort();
        a_cache = tmp;
    }

    if (row_prev1 == NULL)
        row_prev1 = (seq_t *)calloc(newSize, sizeof(seq_t));
    else {
        seq_t *tmp = (seq_t *)realloc(row_prev1, newSize * sizeof(seq_t));
        if (tmp == NULL) abort();
        row_prev1 = tmp;
    }

    if (row_prev2 == NULL)
        row_prev2 = (seq_t *)calloc(newSize, sizeof(seq_t));
    else {
        seq_t *tmp = (seq_t *)realloc(row_prev2, newSize * sizeof(seq_t));
        if (tmp == NULL) abort();
        row_prev2 = tmp;
    }

    if (col_prev1 == NULL)
        col_prev1 = (seq_t *)calloc(newSize, sizeof(seq_t));
    else {
        seq_t *tmp = (seq_t *)realloc(col_prev1, newSize * sizeof(seq_t));
        if (tmp == NULL) abort();
        col_prev1 = tmp;
    }

    if (col_prev2 == NULL)
        col_prev2 = (seq_t *)calloc(newSize, sizeof(seq_t));
    else {
        seq_t *tmp = (seq_t *)realloc(col_prev2, newSize * sizeof(seq_t));
        if (tmp == NULL) abort();
        col_prev2 = tmp;
    }

    allocSize = newSize;
}
#else
/* C++ implementation – use std::vector for safe resizing of
 * complex objects. */
static std::vector<seq_t> a_cache;
static std::vector<seq_t> row_prev1;
static std::vector<seq_t> row_prev2;
static std::vector<seq_t> col_prev1;
static std::vector<seq_t> col_prev2;

static void ensure_capacity(idx_t n)
{
    size_t newSize = (size_t)n + 1;
    if (allocSize >= newSize) return;

    a_cache.resize(newSize);
    row_prev1.resize(newSize);
    row_prev2.resize(newSize);
    col_prev1.resize(newSize);
    col_prev2.resize(newSize);

    allocSize = newSize;
}
#endif

/* ----------------------------------------------------------
 *  Main sequence function
 * ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* sequence undefined for n<1 */

    ensure_capacity(n);

    /* Compute diagonals that have not yet been computed. */
    for (idx_t d = maxComputed + 1; d <= n; ++d)
    {
        bool up = (d % 2 == 0);    /* even diagonals go “up” */
        seq_t prev_diag = 0;

        for (idx_t k = 0; k < d; ++k)
        {
            idx_t row, col;
            seq_t val;

            if (up) {
                row = d - k;      /* row decreasing */
                col = 1 + k;
                if (k == 0) {
                    val = (seq_t)1;
                } else {
                    /* previous element in the diagonal + two
                       previous elements in the same row */
                    val = prev_diag
                          + row_prev1[row]
                          + row_prev2[row];
                }
            } else {
                row = 1 + k;      /* row increasing */
                col = d - k;
                if (k == 0) {
                    val = (seq_t)1;
                } else {
                    /* previous element in the diagonal + two
                       previous elements in the same column */
                    val = prev_diag
                          + col_prev1[col]
                          + col_prev2[col];
                }
            }

            /* Update row and column histories to the newest values. */
            row_prev2[row] = row_prev1[row];
            row_prev1[row] = val;

            col_prev2[col] = col_prev1[col];
            col_prev1[col] = val;

            prev_diag = val;   /* store for the next element */
        }

        a_cache[d] = prev_diag;   /* last element of diagonal d */
    }

    maxComputed = n;
    return a_cache[n];
}

#endif /* A062704_H */
