/*
 * A188587  1-Euler triangle.
 * https://oeis.org/A188587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------
 * Sequence: 1‑Euler triangle (OEIS A188587) flattened row‑by‑row.
 *
 * Recurrence (A188587):
 *   T(n,k) = 0                           if k<0 or k>n
 *   T(n,k) = 1                           if k=0 or k=n or n=2
 *   T(n,k) = (n-k+1)*T(n-1,k-1) + (k+1)*T(n-1,k)
 *                for n>2 and 1≤k<n
 *
 * The function `a(n)` returns the n-th element of the flattened triangle,
 * where n=0 corresponds to T(0,0).
 *
 * ------------------------------------------------------------- */

#include <stdlib.h>

/* Integer square root for unsigned 64‑bit values.  */
static inline idx_t
isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = 1ULL << 32;          /* hi > sqrt(UINT64_MAX) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > x / mid)                  /* avoid overflow */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Return the n-th element of A188587. */
seq_t a(idx_t n)
{
    /* Determine the row (r) and column (k) for index n. */
    idx_t val     = (idx_t)(((unsigned __int128)8 * n) + 1);
    idx_t sq      = isqrt_u64(val);                  /* floor(sqrt(8*n+1)) */
    idx_t r       = (sq - 1) / 2;                    /* r = floor((sqrt(8*n+1)-1)/2) */
    idx_t prevCnt = r * (r + 1) / 2;                /* element count before row r */
    idx_t k       = n - prevCnt;                    /* column within row r */

    /* Columns at the edges or when r ≤ 2 are always 1. */
    if (r <= 2)
        return (seq_t)1;

    /* Allocate two working rows of size r+1. */
    seq_t *prev = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return (seq_t)0; /* error case */
    }

    /* Initialize row 0. */
    prev[0] = (seq_t)1;

    /* Build rows 1 through r. */
    for (idx_t i = 1; i <= r; ++i) {
        /* Row i has i+1 entries (indices 0..i). */
        if (i == 2) {                /* row 2 is all ones */
            for (idx_t j = 0; j <= i; ++j)
                curr[j] = (seq_t)1;
        } else {
            curr[0] = (seq_t)1;
            curr[i] = (seq_t)1;
            for (idx_t j = 1; j < i; ++j) {
                seq_t term1 = ((seq_t)(i - j + 1)) * prev[j - 1];
                seq_t term2 = ((seq_t)(j + 1)) * prev[j];
                curr[j] = term1 + term2;
            }
        }
        /* Prepare for next iteration: prev <- curr, curr <- old prev. */
        seq_t *tmp  = prev;
        prev = curr;
        curr = tmp;
    }

    /* At this point, 'prev' holds row r. */
    seq_t result = prev[k];

    free(prev);
    free(curr);
    return result;
}
