/*
 * A201638  Triangle read by rows, T(n,k) for 0<=k<=n, generalizes the colored
 *          Motzkin paths of A107264.
 * https://oeis.org/A201638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle read by rows. Triangle
 *            T(n,k): T(0,0)=1, T(n,k)=T(n-1,k-1)+3*T(n-1,k)+3*T(n-1,k+1)
 *            (0<=k<=n). The sequence is obtained by enumerating the rows of
 *            this triangle, one after another
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* Find the row index r such that the cumulative number of terms
       up to (and including) row r exceeds n.  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Row 0 contains only the element 1.  */
    if (r == 0)
        return (seq_t)1;

    /* Allocate two temporary rows.  The maximum row length up to r
       is r+1, so we allocate r+2 to be safe for indexing.  */
    seq_t *prev = mem_new(seq_t, (size_t)(r + 2));
    seq_t *cur  = mem_new(seq_t, (size_t)(r + 2));
    if (!prev || !cur) {
        if (prev) mem_free(prev);
        if (cur)  mem_free(cur);
        return (seq_t)0;          /* allocation failure (unlikely) */
    }

    /* Initialize the first row: T(0,0)=1.  */
    for (idx_t i = 0; i < r + 2; ++i)
        prev[i] = (seq_t)0;
    prev[0] = (seq_t)1;

    /* Compute rows 1 through r using the recurrence.  */
    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t k = 0; k <= i; ++k) {
            seq_t left   = (k > 0)        ? prev[k - 1] : (seq_t)0;
            seq_t center = (k <= i - 1)   ? prev[k]     : (seq_t)0;
            seq_t right  = (k + 1 <= i - 1)? prev[k + 1] : (seq_t)0;
            cur[k] = left + 3 * center + 3 * right;
        }
        /* Swap the buffers for the next iteration.  */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    /* Determine the position of n within row r.  */
    idx_t start = r * (r + 1) / 2;      /* first index of row r */
    idx_t k = n - start;
    seq_t result = prev[k];

    mem_free(prev);
    mem_free(cur);
    return result;
}
