/*
 * A096470  Triangle T(n,k), read by rows, formed by setting all entries in
 *          the zeroth column and in the main diagonal ((n,n) entries) to 1
 *          and defining the rest of the entries by the recursion T(n,k) =
 *          T(n-1,k) - T(n,k-1).
 * https://oeis.org/A096470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 39, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  T(0,0)=1;  T(n,0)=1=T(n,n).  
   For 1 ≤ k ≤ n−1:  T(n,k)=T(n−1,k)−T(n,k−1).  
   Sequence a(n) is obtained by reading the triangle row‑wise. */
#include <stdlib.h>
#include <math.h>
#include "../utils/mem.h"

/* internal helper:  find the row index r such that the
   n‑th term lies in row r of the triangle. */
static inline idx_t triangle_row(idx_t n)
{
    /* Solve r(r+1)/2 ≤ n < (r+1)(r+2)/2.
       Using a high‑precision sqrt to avoid overflow. */
    long double v = (long double)n * 8.0L + 1.0L;
    idx_t r = ((idx_t)sqrtl(v) - 1) >> 1;
    /* Correct possible rounding errors. */
    while ((unsigned __int128)r * (r + 1ULL) / 2 > n) --r;
    while ((unsigned __int128)(r + 1) * (r + 2ULL) / 2 <= n) ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Handle the first row directly. */
    if (n == 0) return 1;

    /* Determine which row and column the index n falls into. */
    idx_t r = triangle_row(n);                       /* row number */
    idx_t k = n - (unsigned __int128)r * (r + 1ULL) / 2; /* column index */

    /* Edge cases on the boundary of the triangle. */
    if (k == 0 || k == r) return 1;

    /* Allocate space for two consecutive rows.  Size r+1 is enough
       for all rows up to r. */
    seq_t *prev = mem_new(seq_t, (r + 1));
    seq_t *curr = mem_new(seq_t, (r + 1));
    if (!prev || !curr) return 0;  /* memory allocation failed */

    /* Initialize the first row (row 0). */
    prev[0] = 1;

    /* Build the triangle up to the desired row. */
    for (idx_t i = 1; i <= r; ++i)
    {
        curr[0] = 1;          /* T(i,0) = 1 */
        curr[i] = 1;          /* T(i,i) = 1 */
        for (idx_t j = 1; j < i; ++j)
        {
            curr[j] = prev[j] - curr[j - 1];
        }
        /* Rotate buffers for the next iteration. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* The result is in the current 'prev' buffer. */
    seq_t result = prev[k];

    mem_free(prev);
    mem_free(curr);
    return result;
}
