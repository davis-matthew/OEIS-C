/*
 * A125608  Triangle read by rows: given the left border = the Lucas numbers,
 *          (1, 3, 4, 7, ...), T(n,k) = (n-1,k) + (n-1,k-1).
 * https://oeis.org/A125608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 159, u32 n <= 607, u64 n <= 2239,
 *            u128 n <= 8577, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A125608: Triangle read by rows
 *  Triangle definition
 *      Left border: Lucas numbers L_1=1, L_2=3, L_n = L_{n-1}+L_{n-2}.
 *      Recurrence:   T(n,k) = T(n-1,k) + T(n-1,k-1)    (k>1, n>1)
 *  Sequence a(n) is the triangle read by rows (row1, row2, …), offset 1.
 *
 *  This implementation builds successive rows of the triangle until the
 *  required index n is reached.  The row index r satisfies r(r+1)/2 ≥ n, and
 *  the column within that row is k = n - (r-1)r/2.  The algorithm uses
 *  only a few O(r) temporary arrays and is O(r²) time (r ≈ √(2n)).
 */

#include <stdlib.h>

#ifndef SIZE_T
#define SIZE_T size_t
#endif

seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th element. */
    idx_t r = 1;
    idx_t cumulative = 1;                    /* sum_{i=1}^{r} i */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Allocate two rows of the triangle.  Rows are 1-indexed. */
    SIZE_T sz = (SIZE_T)r + 1;
    seq_t *prev = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *cur  = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!prev || !cur) {                       /* allocation failed */
        return (seq_t)0;                       /* fallback; should not happen */
    }

    /* Keep the two most recent Lucas numbers. */
    seq_t l_prev_prev = (seq_t)1;   /* L1 */
    seq_t l_prev      = (seq_t)3;   /* L2 */
    seq_t lucas = 0;

    for (SIZE_T i = 1; i <= r; ++i) {
        /* Compute Lucas number for the current row. */
        if (i == 1)
            lucas = l_prev_prev;         /* L1 = 1 */
        else if (i == 2)
            lucas = l_prev;              /* L2 = 3 */
        else {
            lucas = l_prev + l_prev_prev;
            l_prev_prev = l_prev;
            l_prev = lucas;
        }

        if (i == 1) {
            prev[1] = lucas;             /* row 1 */
        } else {
            cur[1] = lucas;              /* left border of row i */
            for (SIZE_T k = 2; k <= i; ++k) {
                cur[k] = prev[k] + prev[k - 1];
            }
            /* Swap prev and cur for next iteration. */
            seq_t *tmp = prev;
            prev = cur;
            cur = tmp;
        }
    }

    /* Determine the column within the final row. */
    idx_t prev_cumulative = (r - 1) * r / 2;   /* elements before row r */
    SIZE_T k = (SIZE_T)(n - prev_cumulative); /* 1‑based column index */

    seq_t result = prev[k];

    /* Free temporary storage. */
    free(prev);
    free(cur);

    return result;
}
