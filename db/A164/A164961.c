/*
 * A164961  Triangle read by rows: T[n,m] = quadruple factorials A001813(n) *
 *          binomials A007318(n,m).
 * https://oeis.org/A164961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle read by rows with recurrences * T(0,0)=1,
 *            T(r,k) = (4*(r-1)+2) * (T(r-1,k)+T(r-1,k-1)) * a(n) = T(r,k)
 *            where r is the row containing index n
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t row_from_index(idx_t n)
{
    /* Find the largest r such that r*(r+1)/2 <= n using binary search. */
    idx_t low = 0, high = 1;
    while ((__int128)high * (high + 1) / 2 <= n)
        high <<= 1;                     /* exponentially grow upper bound   */
    idx_t r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 off = (__int128)mid * (mid + 1) / 2;
        if (off <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Determine the row r and the position k within that row. */
    idx_t r = row_from_index(n);
    idx_t offset = (__int128)r * (r + 1) / 2;
    idx_t k = n - offset;

    if (r == 0)                 /* only element is 1 */
        return (seq_t)1;

    /* Allocate two arrays of size r+1 for the recurrence. */
    size_t sz = (size_t)(r + 1);
    seq_t *prev = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!prev || !cur) {        /* allocation failure – unlikely for small r   */
        free(prev);
        free(cur);
        return (seq_t)0;
    }

    /* Base case: row 0 has one entry equal to 1. */
    prev[0] = (seq_t)1;

    /* Build rows 1 … r using the recurrence. */
    for (idx_t row = 1; row <= r; ++row) {
        seq_t factor = (seq_t)(4 * (row - 1) + 2);

        /* k = 0 */
        cur[0] = factor * prev[0];

        /* 1 ≤ k ≤ row‑1 */
        for (idx_t i = 1; i < row; ++i)
            cur[i] = factor * (prev[i] + prev[i - 1]);

        /* k = row */
        cur[row] = factor * prev[row - 1];

        /* Swap the arrays for next iteration. */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* prev now holds the desired row; extract the k‑th element. */
    seq_t result = prev[k];

    free(prev);
    free(cur);
    return result;
}
