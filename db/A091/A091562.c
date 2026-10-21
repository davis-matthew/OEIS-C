/*
 * A091562  Triangle read by rows, related to Pascal's triangle, starting with
 *          1, 0, 0.
 * https://oeis.org/A091562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   T(n, k) = T(n-1, k) + T(n-1, k-1) + T(n-2, k) + T(n-2, k-1) +
 *            T(n-2, k-2) (n ≥ 2, k ≥ 0), with initial rows Row(0) = [1] and
 *            Row(1) = [0, 0]. a(n) is obtained by reading the triangle row by
 *            row (row0, row1, row2, …) and returning the value at position n
 *            in the linear sequence
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <cstddef>
#endif

/* Helper functions for dynamic allocation that work in C and C++.
   In C++ the elements are constructed properly; in C they are
   allocated with malloc. */
static seq_t *alloc_seq(size_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static void free_seq(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Linearised triangle: linear[i] = a(i).  We grow this array as needed. */
static seq_t *linear = (seq_t *)0;
static idx_t linear_size = 0;    /* number of elements in linear */

/* Storage for the two most recent rows of the triangle. */
static seq_t *prev_row     = (seq_t *)0;  /* row r-1 */
static size_t prev_len     = 0;
static seq_t *prev_prev_row = (seq_t *)0; /* row r-2 */
static size_t prev_prev_len = 0;
static idx_t row_count = 0;               /* number of rows already generated */

/* Append the values of one new row to the linear array and update the
   two‑row buffers.  The new row is stored in cur. */
static void add_next_row(void)
{
    idx_t r  = row_count;                 /* current row number   */
    size_t len = (size_t)(r + 1);         /* number of elements in this row   */
    seq_t *cur = alloc_seq(len);

    for (size_t k = 0; k < len; ++k) {
        seq_t val;
        if (r == 0) {                     /* first row */
            val = 1;
        } else if (r == 1) {              /* second row */
            val = 0;
        } else {                          /* recurrence table */
            seq_t sum = 0;
            /* T(r‑1, k) */
            if (k < prev_len) sum += prev_row[k];
            /* T(r‑1, k‑1) */
            if (k >= 1) sum += prev_row[k - 1];
            /* T(r‑2, k) */
            if (k < prev_prev_len) sum += prev_prev_row[k];
            /* T(r‑2, k‑1) */
            if (k >= 1 && (k - 1) < prev_prev_len)
                sum += prev_prev_row[k - 1];
            /* T(r‑2, k‑2) */
            if (k >= 2) sum += prev_prev_row[k - 2];
            val = sum;
        }
        cur[k] = val;
    }

    /* Append cur to the linear array. */
    size_t new_linear_len = (size_t)linear_size + len;
    seq_t *new_linear = alloc_seq(new_linear_len);
    for (size_t i = 0; i < linear_size; ++i)
        new_linear[i] = linear[i];
    for (size_t i = 0; i < len; ++i)
        new_linear[linear_size + i] = cur[i];
    free_seq(linear);
    linear = new_linear;
    linear_size = (idx_t)new_linear_len;

    /* Update row buffers and free the row that is no longer needed. */
    if (row_count == 0) {                /* after computing row 0 */
        prev_row = cur;
        prev_len = len;
    } else if (row_count == 1) {          /* after computing row 1 */
        prev_prev_row = prev_row;
        prev_prev_len = prev_len;
        prev_row = cur;
        prev_len = len;
    } else {                              /* r >= 2 */
        seq_t *old_prev_prev = prev_prev_row;
        prev_prev_row = prev_row;
        prev_prev_len = prev_len;
        prev_row = cur;
        prev_len = len;
        free_seq(old_prev_prev);
    }

    ++row_count;
}

/* Ensure that a(n) has been computed and stored in linear[n]. */
static void ensure_precomputed(idx_t n)
{
    while (linear_size <= n) {
        add_next_row();
    }
}

seq_t a(idx_t n)
{
    ensure_precomputed(n);
    return linear[n];
}
