/*
 * A131980  A coefficient tree from the list partition transform relating
 *          A000129, A000142, A000165, A110327, and A110330.
 * https://oeis.org/A131980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   A131980 computed from the coefficient triangle T(n,k) defined by
 *            T(0,0) = 1 and for n ≥ 0, 0 ≤ k ≤ ⌊(n+1)/2⌋ T(n+1,k) =
 *            (n+2+2k)·T(n,k) + (n+2-2k)·T(n,k-1). The sequence a(n) is
 *            obtained by concatenating the rows of this triangle, row n
 *            containing T(n,0),T(n,1),…,T(n,⌊n/2⌋)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r and column c that contain the nth element. */
    idx_t total = 0;
    idx_t r = 0;
    while (1) {
        idx_t len = r / 2 + 1;        /* number of elements in row r */
        if (total + len > n) break;   /* n lies in this row */
        total += len;
        ++r;
    }
    idx_t c = n - total;              /* column inside row r */

    /* The longest row needed is the current row r.
       Allocate two arrays large enough for that row. */
    idx_t max_len = r / 2 + 1;
    seq_t *prev = (seq_t *)malloc((size_t)max_len * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((size_t)max_len * sizeof(seq_t));
    if (!prev || !curr) return 0;     /* memory allocation failed */

    /* Initialise the base case T(0,0)=1. */
    for (idx_t i = 0; i < max_len; ++i) prev[i] = 0;
    prev[0] = 1;

    /* Build rows up to r using the recurrence. */
    for (idx_t i = 0; i < r; ++i) {
        idx_t len_prev = i / 2 + 1;
        idx_t len_curr = (i + 1) / 2 + 1;
        for (idx_t k = 0; k < len_curr; ++k) {
            seq_t term1 = 0;
            seq_t term2 = 0;
            if (k < len_prev) {
                term1 = (seq_t)( (idx_t)(i) + 2 + 2 * k ) * prev[k];
            }
            if (k > 0) {
                term2 = (seq_t)( (idx_t)(i) + 2 - 2 * k ) * prev[k - 1];
            }
            curr[k] = term1 + term2;
        }
        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[c];           /* a(n) is T(r,c) */

    free(prev);
    free(curr);
    return result;
}
