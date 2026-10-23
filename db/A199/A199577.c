/*
 * A199577  Coefficient triangle of the associated Laguerre polynomials of
 *          order 1.
 * https://oeis.org/A199577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 21, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   ensure_idx_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the a(n) of OEIS A199577 (coefficients of La_n(1;x)).
   Recurrence:  La_0 = 1,
                 La_1 = x – 3,
                 La_n = (x – (2n+1))·La_{n‑1} – n²·La_{n‑2}   (n ≥ 2).
   The array a(0)…a(n) is the flattened triangle of coefficients
   La_n = Σ_{k=0..n} a(n,k)·x^k.  The function works for any seq_t and
   any idx_t.  It is memoised, so repeated calls are very fast.  */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_idx_capacity.h"

/* Helper to compute triangular numbers: r*(r+1)/2 */
static inline idx_t triangular(idx_t r)
{
    return (idx_t)((seq_t)r * ((seq_t)r + 1) / 2);
}

/* Ensure capacity for a seq_t sequence */
static void ensure_seq_capacity(seq_t **seq_ptr, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 64;
        while (new_cap < need) new_cap <<= 1;
        *seq_ptr = (seq_t *)realloc(*seq_ptr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Cached data: flattened coefficient table and row start indices */
static seq_t *flat = NULL;          /* flattened coefficients */
static size_t flat_cap = 0;
static size_t flat_len = 0;         /* number of elements stored */

static idx_t *row_start = NULL;     /* row_start[i] = start idx of row i in flat */
static size_t start_cap = 0;
static size_t start_len = 0;        /* number of rows computed */

/* Compute rows from start_len up to target_row (inclusive) */
static void compute_rows_up_to(idx_t target_row)
{
    for (idx_t i = (idx_t)start_len; i <= target_row; ++i) {
        /* length of new row */
        idx_t len = i + 1;

        /* Temporary storage for the new row */
        seq_t cur[200];
        for (idx_t t = 0; t < len; ++t) cur[t] = 0;

        if (i == 0) {
            cur[0] = 1;
        } else {
            /* Contribution from La_{i-1} */
            idx_t prev_start = row_start[i - 1];
            for (idx_t k = 0; k < i; ++k) {
                seq_t v1 = flat[prev_start + k];
                cur[k + 1] += v1;                                   /* x·La_{i-1} */
                cur[k] -= ((seq_t)(2 * i + 1)) * v1;               /* –(2i+1)·La_{i-1} */
            }
            /* Contribution from La_{i-2} when i ≥ 2 */
            if (i >= 2) {
                idx_t prev2_start = row_start[i - 2];
                for (idx_t k = 0; k < i - 1; ++k) {
                    seq_t v2 = flat[prev2_start + k];
                    cur[k] -= ((seq_t)i * (seq_t)i) * v2;           /* –i²·La_{i-2} */
                }
            }
        }

        /* Append the new row to flat */
        ensure_seq_capacity(&flat, &flat_cap, flat_len + (size_t)len);
        for (idx_t t = 0; t < len; ++t)
            flat[flat_len + t] = cur[t];

        /* Record the start index of this row */
        ensure_idx_capacity(&row_start, &start_cap, start_len + 1);
        row_start[start_len] = flat_len;
        ++start_len;

        flat_len += (size_t)len;
    }
}

seq_t a(idx_t n)
{
    /* Determine the row r such that triangular(r) ≤ n < triangular(r+1) */
    idx_t r = 0;
    while (triangular(r + 1) <= n) ++r;

    /* Compute rows up to r if needed */
    if (start_len <= r)
        compute_rows_up_to(r);

    /* Retrieve the coefficient a(r, k) with k = n – triangular(r) */
    idx_t k = n - triangular(r);
    return flat[row_start[r] + k];
}
