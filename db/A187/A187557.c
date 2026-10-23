/*
 * A187557  Triangle read by rows of products of Stirling numbers of the
 *          second kind (A008277): a(n,k) = S(n,k) S(n+1,k+1).
 * https://oeis.org/A187557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 58, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A187557:  Triangle read by rows of products of Stirling numbers of the second kind
 *
 *  a(n) = S(r,k) * S(r+1,k+1)
 *
 *  where r is the unique integer such that
 *      r*(r+1)/2 <= n < (r+1)*(r+2)/2,
 *  and k = n - r*(r+1)/2.
 *
 *  S(n,k) are Stirling numbers of the second kind, computed by
 *  the recurrence
 *      S(0,0) = 1,  S(n,0) = 0 (n>0)
 *      S(n,k) = k * S(n-1,k) + S(n-1,k-1)   (1 <= k <= n)
 *
 *  The function keeps a static cache of all computed rows so that calls
 *  can be made in any order.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static storage for Stirling rows: S_rows[n][k] = S(n,k) */
static seq_t **S_rows = NULL;          /* Array of pointers to rows */
static size_t S_rows_cap = 0;          /* Allocated capacity of S_rows array */
static idx_t  S_max_row  = 0;          /* Largest row number already computed */

/* Ensure the S_rows array can hold at least (n+1) rows */
static void grow_rows_capacity(idx_t n)
{
    size_t new_cap = S_rows_cap;
    if (new_cap == 0) new_cap = 1;          /* start with size 1 */
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t **new_ptr = (seq_t**)malloc(new_cap * sizeof(seq_t*));
    if (!new_ptr) abort();                 /* Out of memory */

    /* Preserve existing rows, set new entries to NULL */
    for (size_t i = 0; i < new_cap; ++i)
        new_ptr[i] = NULL;
    if (S_rows) {
        for (size_t i = 0; i < S_rows_cap; ++i)
            new_ptr[i] = S_rows[i];
        free(S_rows);
    }
    S_rows      = new_ptr;
    S_rows_cap  = new_cap;
}

/* Compute Stirling rows up to 'n' (inclusive).  Existing rows are kept. */
static void ensure_S(idx_t n)
{
    /* Initialise the array of row pointers if necessary */
    if (!S_rows) grow_rows_capacity(0);

    /* Compute row 0 if it hasn't been done yet */
    if (!S_rows[0]) {
        seq_t *row0 = (seq_t*)malloc(sizeof(seq_t));  /* row 0 has length 1 */
        if (!row0) abort();
        row0[0] = (seq_t)1;                          /* S(0,0) = 1 */
        S_rows[0] = row0;
        S_max_row = 0;
    }

    /* Nothing to do if already up to 'n' */
    if (S_max_row >= n) return;

    /* Ensure enough space for up to row n */
    grow_rows_capacity(n);

    /* Build new rows incrementally */
    for (idx_t row = S_max_row + 1; row <= n; ++row) {
        seq_t *rrow = (seq_t*)malloc((row + 1) * sizeof(seq_t));
        if (!rrow) abort();

        /* row 0 is 0 for n>0, handled below */
        rrow[0] = (seq_t)0;
        for (idx_t k = 1; k <= row; ++k) {
            seq_t term1 = (k <= row - 1) ? ((seq_t)k * S_rows[row - 1][k]) : (seq_t)0;
            seq_t term2 = S_rows[row - 1][k - 1];
            rrow[k] = term1 + term2;
        }
        S_rows[row] = rrow;
    }
    S_max_row = n;
}

/* Return the n-th term of the sequence */
seq_t a(idx_t n)
{
    /* Find the row 'r' such that n lies in this row */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;

    idx_t prev = r * (r + 1) / 2;   /* First index of this row */
    idx_t k    = n - prev;          /* Position inside the row */

    /* Ensure necessary Stirling rows are available */
    ensure_S(r + 1);

    /* Compute the product S(r,k) * S(r+1,k+1) */
    seq_t val = S_rows[r][k] * S_rows[r + 1][k + 1];
    return val;
}
