/*
 * A180085  Eulerian polynomials at nonpositive integers, A_{n}(-n).
 * https://oeis.org/A180085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0}^{n-1} A(n,k) * (-n)^k, where A(n,k) are Eulerian
 *            numbers. The implementation builds Eulerian numbers row by row
 *            using the recurrence A(n,k) = (k+1)*A(n-1,k) + (n-k)*A(n-1,k-1)
 *            and accumulates the required value
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 14, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */

static seq_t *values = NULL;          /* a(0), a(1), ... */
static idx_t values_cap = 0;          /* allocated size of values */

static seq_t *prev_row = NULL;        /* Eulerian row for n-1 */
static seq_t *curr_row = NULL;        /* Eulerian row for current n */
static idx_t row_cap = 0;             /* allocated size of Eulerian row buffer */

static idx_t last_computed = 0;       /* highest n for which a(n) and prev_row are ready */

/* Ensure the values array can hold at least (cap) elements. */
static void ensure_values_capacity(idx_t cap) {
    if (values_cap < cap) {
        void *ptr = realloc(values, cap * sizeof(seq_t));
        if (!ptr) { /* allocation failed; abort */
            abort();
        }
        values = (seq_t *)ptr;
        values_cap = cap;
    }
}

/* Ensure the row buffer can hold at least (cap) elements. */
static void ensure_row_capacity(idx_t cap) {
    if (row_cap < cap) {
        void *ptr = realloc(prev_row, cap * sizeof(seq_t));
        if (!ptr) abort();
        prev_row = (seq_t *)ptr;
        ptr = realloc(curr_row, cap * sizeof(seq_t));
        if (!ptr) abort();
        curr_row = (seq_t *)ptr;
        row_cap = cap;
    }
}

seq_t a(idx_t n) {
    /* Handle the trivial case n == 0 without initializing the buffers. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* First call: initialise buffers and base values. */
    if (!values) {
        /* Allocate space for a(0) and a(1). */
        ensure_values_capacity(2);
        values[0] = (seq_t)1;
        values[1] = (seq_t)1;

        /* Allocate Eulerian rows for n = 1. */
        ensure_row_capacity(1);
        prev_row[0] = (seq_t)1;     /* A(1,0) = 1 */
        last_computed = 1;
    }

    /* If the requested n has already been computed, return it. */
    if (n <= last_computed) {
        return values[n];
    }

    /* Otherwise compute successive rows up to n. */
    for (idx_t nb = last_computed + 1; nb <= n; ++nb) {
        /* Prepare buffers for the new row. */
        idx_t row_len = nb;            /* length of A(nb,*) is nb */
        ensure_row_capacity(row_len);

        curr_row[0] = (seq_t)1;
        curr_row[row_len - 1] = (seq_t)1;

        /* Compute interior Eulerian entries using the recurrence. */
        for (idx_t k = 1; k < row_len - 1; ++k) {
            seq_t term1 = (seq_t)(k + 1) * prev_row[k];
            seq_t term2 = (seq_t)(nb - k) * prev_row[k - 1];
            curr_row[k] = term1 + term2;
        }

        /* Accumulate a(nb) = Σ A(nb,k) * (-nb)^k. */
        seq_t sum = (seq_t)0;
        seq_t pow = (seq_t)1;                /* (-nb)^0 */
        seq_t neg_n = -(seq_t)nb;
        for (idx_t k = 0; k < row_len; ++k) {
            sum += curr_row[k] * pow;
            pow *= neg_n;
        }

        /* Store the result and update the buffers. */
        ensure_values_capacity(nb + 1);
        values[nb] = sum;

        /* Swap prev and curr rows for the next iteration. */
        seq_t *tmp = prev_row;
        prev_row = curr_row;
        curr_row = tmp;
    }

    last_computed = n;
    return values[n];
}
