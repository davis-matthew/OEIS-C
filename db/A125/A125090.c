/*
 * A125090  Triangle read by rows: T(0,0)=1; for 0<=k<=n, n>=1, T(n,k) is the
 *          coefficient of x^(n-k) in the monic characteristic polynomial of
 *          the tridiagonal n X n matrix with diagonal (0,1,1,...) and super-
 *          and subdiagonals (1,1,1,...).
 * https://oeis.org/A125090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 83, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  f(0,x)=1,  f(1,x)=x,
   f(n,x) = (x-1)f(n-1,x) - f(n-2,x)   (n>=2).
   T(n,k) is the coefficient of x^(n-k) in f(n,x).
   The sequence a(n) is the triangle read by rows, with offset 1
   (a(1)=T(0,0)=1).  This function computes a(n) for any n>=1. */

/* Standard headers */
#include <stdlib.h>
#include <stdint.h>

/* Function required by the specification */
seq_t a(idx_t n)
{
    /* Safety for undefined index 0 */
    if (n == 0) return (seq_t)0;

    /* Find the row r such that the n-th term lies in row r
       (rows are numbered 0,1,2,..., and each row has length r+1).
       The cumulative number of terms up to row r is (r+1)(r+2)/2.
       We locate the smallest r with that value >= n.               */
    idx_t low = 0;
    idx_t high = 1;
    /* Expand high until it's large enough */
    while ( (unsigned __int128)(high + 1) * (high + 2) / 2 < n )
        high <<= 1;
    /* Binary search */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 cum = (unsigned __int128)(mid + 1) * (mid + 2) / 2;
        if (cum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                                /* row index */

    /* Position within the row (0‑based) */
    unsigned __int128 before = (unsigned __int128)r * (r + 1) / 2;
    idx_t pos_in_row = (idx_t)(n - before - 1);   /* 0 <= pos_in_row <= r */
    idx_t k = pos_in_row;                         /* column index */

    /* Quick answer for the first row */
    if (r == 0)
        return (seq_t)1;                          /* T(0,0)=1 */

    /* Allocate three working arrays of length r+1 (maximum needed).   */
    idx_t max_len = r + 1;
    seq_t *prev2 = (seq_t *)malloc(max_len * sizeof(seq_t));
    seq_t *prev  = (seq_t *)malloc(max_len * sizeof(seq_t));
    seq_t *cur   = (seq_t *)malloc(max_len * sizeof(seq_t));
    if (!prev2 || !prev || !cur) {
        /* Allocation failure – return 0 as a safe fallback   */
        if (prev2) free(prev2);
        if (prev)  free(prev);
        if (cur)   free(cur);
        return (seq_t)0;
    }

    /* Initialise f(0,x) and f(1,x).  All other entries are zero. */
    for (idx_t i = 0; i < max_len; ++i) {
        prev2[i] = 0;
        prev[i]  = 0;
        cur[i]   = 0;
    }
    prev2[0] = (seq_t)1;   /* f(0,x) = 1            */
    prev[0]  = (seq_t)0;   /* f(1,x) = x            */
    prev[1]  = (seq_t)1;

    /* Build polynomials up to f(r,x)   */
    for (idx_t row = 2; row <= r; ++row) {
        idx_t curSize = row + 1;
        /* Zero cur (only needed for indices < curSize) */
        for (idx_t i = 0; i < curSize; ++i)
            cur[i] = 0;

        /* x * prev   → shift prev by one degree  */
        for (idx_t i = 0; i < row; ++i)
            cur[i + 1] = prev[i];

        /* (x-1) * prev   → subtract prev from cur[0..row-1]   */
        for (idx_t i = 0; i < row; ++i)
            cur[i] -= prev[i];

        /* subtract prev2   (only indices 0..row-2 exist)      */
        for (idx_t i = 0; i < row - 1; ++i)
            cur[i] -= prev2[i];

        /* Rotate buffers for next iteration   */
        seq_t *tmp = prev2;
        prev2 = prev;
        prev = cur;
        cur = tmp;
    }

    /* The needed coefficient is at index (r - k)   */
    seq_t answer = prev[r - k];

    /* Free allocated memory */
    free(prev2);
    free(prev);
    free(cur);

    return answer;
}
