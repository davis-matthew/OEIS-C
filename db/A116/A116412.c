/*
 * A116412  Riordan array ((1+x)/(1-2x),x(1+x)/(1-2x)).
 * https://oeis.org/A116412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 67, u32 n <= 215, u64 n <= 791,
 *            u128 n <= 3024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(0,0)=1, T(1,0)=3, T(1,1)=1.
   For n≥2:
      T(n,0) = 2·T(n‑1,0);
      1≤k≤n‑1:   T(n,k) = 2·T(n‑1,k) + T(n‑1,k‑1) + T(n‑2,k‑1);
      T(n,n)=1.
   Sequence a(n) lists the table entries in row‑major order,
   i.e. a(n)=T(r,k) where r = floor((sqrt(8·n+1)-1)/2) and k = n – r·(r+1)/2. */

#include <stdlib.h>
#include <stddef.h>

/* Integer square root, floor(sqrt(x)) for non‑negative x. */
static idx_t floor_sqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid != 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Static cache of rows of the triangle.  rows[i] points to an array of length i+1. */
    static seq_t **rows = NULL;
    static size_t max_rows = 0;

    /* Find row r and column k for index n. */
    idx_t r = (floor_sqrt(8 * n + 1) - 1) / 2;
    idx_t k = n - r * (r + 1) / 2;

    /* Grow the cache if needed. */
    if ((size_t)r >= max_rows) {
        size_t new_max = (size_t)r + 1;
        seq_t **tmp = (seq_t**)realloc(rows, new_max * sizeof(seq_t*));
        if (!tmp && new_max > 0) abort();   /* allocation failure (unrecoverable) */
        rows = tmp;

        for (size_t i = max_rows; i < new_max; ++i) {
            seq_t *row = (seq_t*)calloc(i + 1, sizeof(seq_t));
            if (!row) abort();
            rows[i] = row;

            if (i == 0) {
                row[0] = (seq_t)1;
            } else if (i == 1) {
                row[0] = (seq_t)3;
                row[1] = (seq_t)1;
            } else {
                /* T(i,0) = 2*T(i-1,0) */
                row[0] = (seq_t)2 * rows[i - 1][0];

                /* 1 <= k <= i-1 */
                for (idx_t kk = 1; kk < (idx_t)i; ++kk) {
                    /* T(i,kk) = 2*T(i-1,kk) + T(i-1,kk-1) + T(i-2,kk-1) */
                    seq_t val = (seq_t)2 * rows[i - 1][kk];
                    val += rows[i - 1][kk - 1];
                    val += rows[i - 2][kk - 1];
                    row[kk] = val;
                }

                /* T(i,i) = 1 */
                row[i] = (seq_t)1;
            }
        }
        max_rows = new_max;
    }

    return rows[(size_t)r][(size_t)k];
}
