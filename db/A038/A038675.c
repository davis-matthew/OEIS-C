/*
 * A038675  Triangle read by rows: T(n,k)=A(n,k)*binomial(n+k-1,n), where
 *          A(n,k) are the Eulerian numbers (A008292).
 * https://oeis.org/A038675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   alloc_seq_array, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: T(n,k) = A(n,k) * binomial(n+k-1,n)
   where A(n,k) are the Eulerian numbers (A008292).
   The sequence a(1),a(2),... is the row‐wise flattening of T. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t n); */
#include "../utils/binomial.h"          /* seq_t binomial(idx_t n, idx_t k); */

/* Cached Eulerian rows: eulerian[row][k], 1 ≤ k ≤ row. */
static seq_t **eulerian = NULL;
static idx_t max_row_allocated = 0;

/* Ensure that Eulerian numbers up to the given row are computed. */
static void ensure_rows(idx_t rows)
{
    if (rows <= max_row_allocated)
        return;

    /* Resize the row pointer array. */
    seq_t **new_ptr = (seq_t **)realloc(eulerian, (rows + 1) * sizeof(seq_t *));
    eulerian = new_ptr;

    /* Compute new rows one by one. */
    for (idx_t r = max_row_allocated + 1; r <= rows; ++r) {
        /* Allocate space for row r; we use indices 1 … r. */
        eulerian[r] = alloc_seq_array((size_t)(r + 1));
        if (r == 1) {
            eulerian[1][1] = (seq_t)1;
        } else {
            eulerian[r][1] = (seq_t)1;
            eulerian[r][r] = (seq_t)1;
            /* Fill interior entries using the correct recurrence:
               A(r,k) = k * A(r-1,k) + (r-k+1) * A(r-1,k-1). */
            for (idx_t k = 2; k < r; ++k) {
                seq_t term1 = eulerian[r-1][k] * (seq_t)k;
                seq_t term2 = eulerian[r-1][k-1] * (seq_t)(r - k + 1);
                eulerian[r][k] = term1 + term2;
            }
        }
    }
    max_row_allocated = rows;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)         /* the sequence is offset 1 by OEIS definition */
        return (seq_t)0;

    /* Determine the row r such that sum_{i=1..r} i >= n. */
    idx_t r = 1;
    idx_t cumulative = 1;            /* sum of 1..r */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Position k within the row (1‑based). */
    idx_t prev_cum = cumulative - r;   /* sum of 1..(r-1) */
    idx_t k = n - prev_cum;            /* 1 ≤ k ≤ r */

    /* Make sure Eulerian numbers for this row are ready. */
    ensure_rows(r);

    /* Compute the triangle entry T(r,k). */
    seq_t bin = binomial(r + k - 1, r);
    return eulerian[r][k] * bin;
}
