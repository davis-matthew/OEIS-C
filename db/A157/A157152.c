/*
 * A157152  Triangle T(n, k, m) = (m*(n-k) + 1)*T(n-1, k-1, m) + (m*k +
 *          1)*T(n-1, k, m) - m*k*(n-k)*T(n-2, k-1, m) with T(n, 0, m) = T(n,
 *          n, m) = 1 and m = 1, read by rows.
 * https://oeis.org/A157152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n-th term of the OEIS triangle A157152 read by rows,
 *            i.e. T(0,0)=1, T(n,0)=T(n,n)=1, T(n,k) = (n-k+1) * T(n-1,k-1) +
 *            (k+1) * T(n-1,k) - k*(n-k) * T(n-2,k-1) for 0<k<n. a(n) is the
 *            linearised index of that triangle
 * Range:     u8 n <= 22, u16 n <= 48, u32 n <= 111, u64 n <= 285,
 *            u128 n <= 757, big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/alloc_seq.h"

static seq_t **tri_rows = NULL;   /* pointers to each row of the triangle */
static idx_t rows_capacity = 0;   /* how many rows are currently allocated */

/* Return the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static inline idx_t find_row(idx_t n) {
    double d = sqrt((double)(1 + 8.0 * (double)n));
    idx_t r = (idx_t)((d - 1.0) / 2.0);
    /* correct possible round‑off errors */
    while (r * (r + 1) / 2 > n) --r;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    return r;
}

/* Main sequence routine */
seq_t a(idx_t n) {
    /* Determine which row and column the term lies in */
    idx_t r = find_row(n);
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Ensure that all rows up to 'r' have been computed */
    if (rows_capacity <= r) {
        /* Resize the array of row pointers */
        seq_t **tmp = (seq_t **)
            realloc(tri_rows, sizeof(seq_t *) * (r + 1));
        if (!tmp) abort();   /* allocation failure */
        tri_rows = tmp;

        /* Compute new rows incrementally */
        for (idx_t rr = rows_capacity; rr <= r; ++rr) {
            /* Allocate storage for the current row */
            seq_t *curr = alloc_seq(rr + 1);
            if (!curr) abort();

            tri_rows[rr] = curr;

            for (idx_t kk = 0; kk <= rr; ++kk) {
                if (kk == 0 || kk == rr) {
                    curr[kk] = (seq_t)1;
                } else {
                    seq_t t1 = ((seq_t)(rr - kk) + 1) *
                               tri_rows[rr - 1][kk - 1];
                    seq_t t2 = ((seq_t)kk + 1) *
                               tri_rows[rr - 1][kk];
                    seq_t t3 = ((seq_t)kk * (seq_t)(rr - kk)) *
                               tri_rows[rr - 2][kk - 1];
                    curr[kk] = t1 + t2 - t3;
                }
            }
        }
        rows_capacity = r + 1;
    }

    return tri_rows[r][k];
}
