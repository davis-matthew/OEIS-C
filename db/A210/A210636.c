/*
 * A210636  Riordan array ((1-x)/(1-2*x-x^2), x*(1+x)/(1-2*x-x^2)).
 * https://oeis.org/A210636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n‑th entry in the flattened Riordan array defined by
 *            the triangle T(n,k) (0 ≤ k ≤ n) with T(0,0) = 1 T(1,0) = T(1,1)
 *            = 1 T(n,k) = 2·T(n‑1,k) + T(n‑1,k‑1) + T(n‑2,k) + T(n‑2,k‑1) and
 *            T(n,k)=0 if k<0 or k>n. The entries are listed in row‑major
 *            order starting at n=0. Example rows: 1 1,1...
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
# include <cstdint>
#endif

/* Helper to allocate a new row of length len and zero‑initialise it. */
static seq_t* allocate_row(size_t len)
{
#ifdef __cplusplus
    return new seq_t[len]();                /* zero‑init in C++ */
#else
    return (seq_t*)calloc(len, sizeof(seq_t)); /* zero‑init in C */
#endif
}

/* Cache of already computed rows.  rows[r] points to the array of
   values of T(r,0)…T(r,r).  max_row is the largest row index
   currently stored. */
static seq_t** rows = NULL;
static idx_t   max_row = 0;

/* Compute rows up to r_needed (inclusive) using the recurrence
   and store them in the global array `rows`. */
static void ensure_rows(idx_t r_needed)
{
    if (rows == NULL) {                         /* first call */
        rows     = (seq_t**)malloc(2 * sizeof(seq_t*));
        rows[0]  = allocate_row(1);
        rows[0][0] = (seq_t)1;
        rows[1]  = allocate_row(2);
        rows[1][0] = (seq_t)1;
        rows[1][1] = (seq_t)1;
        max_row  = 1;
    }

    if (r_needed <= max_row) return;

    rows = (seq_t**)realloc(rows, (r_needed + 1) * sizeof(seq_t*));

    for (idx_t r = max_row + 1; r <= r_needed; ++r) {
        seq_t* cur = allocate_row(r + 1);        /* row r has r+1 entries */
        for (idx_t k = 0; k <= r; ++k) {
            seq_t val = (seq_t)0;

            /* T(r-1,k) */
            if (k <= r - 1)
                val += ((seq_t)2) * rows[r - 1][k];

            /* T(r-1,k-1) */
            if (k > 0)
                val += rows[r - 1][k - 1];

            /* T(r-2,k) */
            if (k <= r - 2)
                val += rows[r - 2][k];

            /* T(r-2,k-1) */
            if (k > 0 && (k - 1) <= r - 2)
                val += rows[r - 2][k - 1];

            cur[k] = val;
        }
        rows[r] = cur;
    }
    max_row = r_needed;
}

/* Public function: a(n) returns the n‑th term of A210636. */
seq_t a(idx_t n)
{
    /* Find the row r that contains index n.  The total number of
       entries up to row r is (r+1)(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;

    /* Column k within that row. */
    idx_t k = n - r * (r + 1) / 2;

    ensure_rows(r);                 /* compute rows up to r if needed */
    return rows[r][k];
}
