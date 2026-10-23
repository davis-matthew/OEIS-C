/*
 * A144502  Square array read by antidiagonals upwards: T(n,k) is the number
 *          of scenarios for the gift exchange problem in which each gift can
 *          be stolen at most once, when there are n gifts in the pool and k
 *          gifts (not yet frozen) in peoples' hands.
 * https://oeis.org/A144502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 77, u64 n <= 189,
 *            u128 n <= 464, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A144502: Square array read by antidiagonals upwards.  
 * T(0,k) = 1                                 (k >= 0)
 * T(n,k) = k * T(n,k-1) + T(n-1,k+1)         (n >= 1, k >= 0)
 * a(n) = T[d-r][r] where
 *   d = floor((sqrt(8*n+1)-1)/2)   // largest d with d*(d+1)/2 <= n
 *   r = n - d*(d+1)/2
 *   (both d and r are idx_t)
 *
 * The implementation keeps the smallest table that contains all
 * values needed for the largest d requested so far.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t **T = NULL;        /* 2‑D table of T(n,k) values            */
static idx_t  CUR_D = 0;        /* maximum d for which T is valid        */
static size_t COLS  = 0;        /* columns per row (2*CUR_D+1)           */

/* ------------------------------------------------------------------ */
/*  Find the largest d such that d*(d+1)/2 <= n.  Uses 128‑bit ints   */
static idx_t get_diagonal(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;
    /* Expand high until the triangle number exceeds n. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;
    /* Binary search [low,high-1] */
    while (low < high - 1) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* ------------------------------------------------------------------ */
/*  Recreate the table up to diagonal d (inclusive).  The table has   */
/*  (d+1) rows, each with (2*d+1) columns.  Row 0 consists of 1’s.   */
static void recompute_table(idx_t d)
{
    /* Free any old table.  T may be NULL on first call.           */
    if (T != NULL) {
        for (idx_t i = 0; i <= CUR_D; ++i)
            free(T[i]);
        free(T);
    }

    CUR_D = d;
    COLS  = 2 * d + 1;
    T = (seq_t **)malloc((d + 1) * sizeof(seq_t *));
    for (idx_t i = 0; i <= d; ++i)
        T[i] = (seq_t *)calloc(COLS, sizeof(seq_t));   /* zeroed init */

    /* Row 0 is all 1’s. */
    for (size_t k = 0; k < COLS; ++k)
        T[0][k] = (seq_t)1;

    /* Subsequent rows using the recurrence. */
    for (idx_t n = 1; n <= d; ++n) {
        size_t max_k = 2 * d - n;            /* up to this column */
        for (size_t k = 0; k <= max_k; ++k) {
            seq_t left  = (k ? ((seq_t)k * T[n][k - 1]) : (seq_t)0);
            seq_t right = T[n - 1][k + 1];
            T[n][k] = left + right;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Public entry point.                                               */
seq_t a(idx_t n)
{
    /* First compute the diagonal index d for this n. */
    idx_t d = get_diagonal(n);

    /* Ensure that the global table contains at least this diagonal. */
    if (T == NULL || d > CUR_D)
        recompute_table(d);

    /* Convert n to coordinates in the triangular array. */
    idx_t countPrev = (idx_t)((unsigned __int128)d * (d + 1) / 2);
    idx_t r    = n - countPrev;   /* 0 <= r <= d                     */
    idx_t i    = d - r;           /* row index in T                   */
    idx_t j    = r;               /* column index in T                */

    return T[i][j];
}
