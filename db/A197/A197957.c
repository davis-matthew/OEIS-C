/*
 * A197957  Odd-index Fibonacci partition triangle read by rows.
 * https://oeis.org/A197957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A197957: Odd-index Fibonacci partition triangle read by rows.
 * Recurrence (for 1 ≤ n, 0 ≤ i < n):
 *   d(0,n) = d(n-1,n) = 1
 *   d(i,n) = 2*d(i-1,n-1) + d(i,n-1) - d(i-1,n-2)   if 0 < i ≤ n/2
 *   d(i,n) = d(i-1,n-1) + 2*d(i,n-1) - d(i-1,n-2)   if n/2 < i < n
 * The sequence a(n) is the triangle read row‑by‑row, starting with
 * a(1)=1.  The implementation builds only the rows that are
 * required for the requested index, using the recurrence above.
 */

#include <stdlib.h>
#include <stdint.h>

/* Find the row number r such that T(r-1) < n ≤ T(r), where
 * T(r) = r*(r+1)/2.  For n up to 10 000 a simple linear scan is
 * fast enough (r ≤ 141).  The function works for any n ≥ 1. */
static inline idx_t _row_of(idx_t n)
{
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Determine the row r and the position k within the row (0‑based). */
    idx_t r = _row_of(n);
    idx_t prevTri = ((r - 1) * r) / 2;      /* sum of lengths of rows < r */
    idx_t k = n - prevTri - 1;              /* 0 ≤ k < r */

    /* Row 1 : single element 1 */
    if (r == 1)
        return (seq_t)1;

    /* Row 2 : two elements 1, 1 */
    if (r == 2)
        return (seq_t)1;                    /* all elements are 1 */

    /* Allocate the first two rows (row 1 and row 2). */
    seq_t *prev2 = (seq_t *)malloc(sizeof(seq_t));   /* row r-2, size 1 */
    prev2[0] = (seq_t)1;                               /* d(0,1) = 1 */

    seq_t *prev1 = (seq_t *)calloc(2, sizeof(seq_t));  /* row r-1, size 2 */
    prev1[0] = prev1[1] = (seq_t)1;                    /* d(0,2)=d(1,2)=1 */

    /* Build rows from 3 up to r. */
    for (idx_t cur = 3; cur <= r; ++cur) {
        seq_t *newrow = (seq_t *)calloc(cur, sizeof(seq_t));
        newrow[0] = newrow[cur - 1] = (seq_t)1;              /* ends are 1 */

        for (idx_t i = 1; i < cur - 1; ++i) {
            if (i <= cur / 2) {
                /* d(i,cur) = 2*d(i-1,cur-1) + d(i,cur-1) - d(i-1,cur-2) */
                newrow[i] = (seq_t)2 * prev1[i - 1] + prev1[i] - prev2[i - 1];
            } else {
                /* d(i,cur) = d(i-1,cur-1) + 2*d(i,cur-1) - d(i-1,cur-2) */
                newrow[i] = prev1[i - 1] + (seq_t)2 * prev1[i] - prev2[i - 1];
            }
        }

        /* Shift rows: prev2 <- prev1, prev1 <- newrow. */
        free(prev2);
        prev2 = prev1;
        prev1 = newrow;
    }

    /* prev1 now holds row r.  Retrieve the requested element. */
    seq_t result = prev1[k];
    free(prev2);
    free(prev1);
    return result;
}
