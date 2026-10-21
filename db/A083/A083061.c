/*
 * A083061  Triangle of coefficients of a companion polynomial to the Gandhi
 *          polynomial.
 * https://oeis.org/A083061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(0,x)=1, T(i,x) = (2x+1)*(x+1)*T(i-1,x+1) – 2x²*T(i-1,x) (i>0).
 *            a(n) is the coefficient of x^j in T(i,x) where i(i+1)/2 ≤ n <
 *            (i+1)(i+2)/2 and j = n – i(i+1)/2. The sequence is the row‑wise
 *            flattening of the polynomial coefficients. Initial values:
 *            a(0)=1. All other values are obtained by the re...
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   compute_shift
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compute_shift.h"

/* --------------------------------------------------------------------- */
/*  Dynamic storage for the rows of the triangle.  Each rows[i] points
    to an array of (i+1) seq_t elements representing T(i,x).  rows_len
    is the number of rows currently stored (rows[0]..rows[rows_len-1]). */
static seq_t **rows = NULL;
static size_t rows_len = 0;   /* number of rows stored */
static size_t rows_cap = 0;   /* capacity of the rows array */

/* --------------------------------------------------------------------- */
/*  Ensure that rows array has at least capacity 'cap'.  Called when
    a new row is to be allocated. */
static void ensure_rows_capacity(size_t cap)
{
    if (cap <= rows_cap) return;
    size_t new_cap = rows_cap == 0 ? 1 : rows_cap * 2;
    if (new_cap < cap) new_cap = cap;
    seq_t **new_rows = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
    if (!new_rows) exit(1);  /* allocation failure */
    rows = new_rows;
    rows_cap = new_cap;
}

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/*  Compute the next row of the triangle given the previous row. */
static void compute_next_row(idx_t row_index)
{
    idx_t prev_len = row_index;                /* previous row length */
    const seq_t *prev = rows[row_index - 1];   /* previous row pointer */

    /* Allocate temporary arrays */
    seq_t *shift = (seq_t *)malloc(prev_len * sizeof(seq_t));
    seq_t *r     = (seq_t *)malloc((prev_len + 1) * sizeof(seq_t));
    seq_t *next  = (seq_t *)malloc((prev_len + 1) * sizeof(seq_t));

    /* 1. Shift coefficients: Tprev(x+1) */
    compute_shift(prev, prev_len, shift);

    /* 2. Multiply by (x+1): r[t] = shift[t] + shift[t-1] (if t>0) */
    for (idx_t t = 0; t <= prev_len; ++t) {
        seq_t v1 = (t < prev_len) ? shift[t] : 0;
        seq_t v2 = (t > 0) ? shift[t - 1] : 0;
        r[t] = v1 + v2;
    }

    /* 3. Multiply by (2x+1) and subtract 2x²*Tprev: */
    for (idx_t t = 0; t <= prev_len; ++t) {
        seq_t term = r[t] + (t > 0 ? 2 * r[t - 1] : 0);
        if (t >= 2) term -= 2 * prev[t - 2];
        next[t] = term;
    }

    /* Store new row */
    ensure_rows_capacity(row_index + 1);
    rows[row_index] = next;

    /* Clean up */
    free(shift);
    free(r);
}

/* --------------------------------------------------------------------- */
/*  Make sure that rows[0..r] are computed.  Row 0 is the base case. */
static void ensure_rows(idx_t r)
{
    /* Allocate rows array on first use */
    if (rows == NULL) {
        rows = (seq_t **)malloc(sizeof(seq_t *));
        rows_cap = 1;
        rows_len = 0;
    }

    /* Compute rows until we have row r */
    while (rows_len <= r) {
        if (rows_len == 0) {
            /* Base case: T(0,x) = 1 -> row 0 has one element 1 */
            seq_t *row0 = (seq_t *)malloc(sizeof(seq_t));
            row0[0] = 1;
            ensure_rows_capacity(1);
            rows[0] = row0;
        } else {
            compute_next_row(rows_len);
        }
        ++rows_len;
    }
}

/* --------------------------------------------------------------------- */
/*  Main entry point for the sequence.  Computes a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    /* Find the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Ensure rows up to r are available */
    ensure_rows(r);

    /* Determine j = position within the row */
    idx_t offset = r * (r + 1) / 2;
    idx_t j = n - offset;

    return rows[r][j];
}
