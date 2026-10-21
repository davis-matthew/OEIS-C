/*
 * A059576  Summatory Pascal triangle T(n,k) (0 <= k <= n) read by rows. Top
 *          entry is 1. Each entry is the sum of the parallelogram above it.
 * https://oeis.org/A059576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl, nice
 * Formula:   a(n) is the nth flattened entry of the summatory Pascal triangle
 *            T(n,k) (0 ≤ k ≤ n). The entry is found as: T(r,k) = U(r-k, k)
 *            where U satisfies U(0,0) = 1 for i>0 or j>0: U(i,j) = U(i-1,j) +
 *            U(i,j-1) - U(i-1,j-1) (derived from 2*U(i,j) = Σ_{x≤i,y≤j}
 *            U(x,y) ) The flattened index n is mapped to ro...
 * Range:     u8 n <= 28, u16 n <= 69, u32 n <= 237, u64 n <= 794,
 *            u128 n <= 2883, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
#include <stdlib.h>  /* malloc, free, abort */

static idx_t  computedMax = 0;   /* highest r already computed       */
static seq_t *S          = NULL; /* cumulative sum table S[i*(max+1)+j] */

/* --------------------------------------------------------------------- */
/* Return the row index r that contains the flattened index n.          */
/* The row r satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2.                      */
static inline idx_t
row_of(idx_t n)
{
    idx_t r = 0;
    /* Cheap integer search – fast enough for r up to a few thousand.   */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    return r;
}

/* --------------------------------------------------------------------- */
/* Build the cumulative table S up to dimension maxR (inclusive).         */
/* The table holds (maxR+1)² entries: S[i][j] = Σ_{x≤i,y≤j} U(x,y).     */
/* --------------------------------------------------------------------- */
static void
build_table(idx_t maxR)
{
    size_t dim   = (size_t)(maxR + 1);
    size_t size  = dim * dim;
    seq_t *newS  = (seq_t *)malloc(size * sizeof(seq_t));
    if (!newS) abort();

    /* Free previous table if any. */
    if (S) free(S);
    S = newS;

    /* Compute table row by row. */
    for (idx_t i = 0; i <= maxR; ++i) {
        for (idx_t j = 0; j <= maxR; ++j) {
            size_t idx = (size_t)i * dim + j;
            if (i == 0 && j == 0) {
                newS[idx] = (seq_t)1;          /* S[0][0] = U(0,0) */
                continue;
            }
            seq_t s_up = (i > 0) ?
                newS[(size_t)(i - 1) * dim + j] : (seq_t)0;
            seq_t s_left = (j > 0) ?
                newS[(size_t)i * dim + (j - 1)] : (seq_t)0;
            seq_t s_ul = (i > 0 && j > 0) ?
                newS[(size_t)(i - 1) * dim + (j - 1)] : (seq_t)0;
            /* U(i,j) = s_up + s_left - s_ul */
            seq_t u = s_up + s_left - s_ul;
            /* S(i,j) = s_up + s_left - s_ul + u */
            newS[idx] = s_up + s_left - s_ul + u;
        }
    }
    computedMax = maxR;
}

/* --------------------------------------------------------------------- */
/* Return U(i,j) using the cumulative table that has already been built.  */
/* Assumes i, j ≤ computedMax.                                            */
/* --------------------------------------------------------------------- */
static inline seq_t
U_value(idx_t i, idx_t j)
{
    if (i == 0 && j == 0) return (seq_t)1;
    size_t dim = (size_t)(computedMax + 1);
    seq_t s_up = (i > 0) ?
        S[(size_t)(i - 1) * dim + j] : (seq_t)0;
    seq_t s_left = (j > 0) ?
        S[(size_t)i * dim + (j - 1)] : (seq_t)0;
    seq_t s_ul = (i > 0 && j > 0) ?
        S[(size_t)(i - 1) * dim + (j - 1)] : (seq_t)0;
    return s_up + s_left - s_ul;
}

/* --------------------------------------------------------------------- */
/* Public entry point – returns a(n).                                      */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t tot = r * (r + 1) / 2;
    idx_t c   = n - tot;           /* 0 ≤ c ≤ r                        */
    idx_t i   = r - c;              /* indices for U(i,c)                */
    idx_t j   = c;

    if (i > computedMax || j > computedMax) {
        /* Grow table to at least r (since we need up to r for both i and j). */
        build_table(r);
    }

    return U_value(i, j);
}
