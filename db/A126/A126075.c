/*
 * A126075  Triangle T(n,k), 0 <= k <= n, read by rows, defined by: T(0,0)=1,
 *          T(n,k)=0 if k < 0 or if k > n, T(n,0) = 2*T(n-1,0) + T(n-1,1),
 *          T(n,k) = T(n-1,k-1) + T(n-1,k+1) for k >= 1.
 * https://oeis.org/A126075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained by reading the triangle T(n,k) row‑by‑row,
 *            where T(0,0) = 1, T(n,k) = 0 for k<0 or k>n, T(n,0) = 2*T(n-1,0)
 *            + T(n-1,1), T(n,k) = T(n-1,k-1) + T(n-1,k+1) for k>=1. That is,
 *            a(0)=1, a(1)=2, a(2)=1, a(3)=5, a(4)=2, …
 * Range:     u8 n <= 27, u16 n <= 90, u32 n <= 324, u64 n <= 1224,
 *            u128 n <= 1274, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low  = 0;
    idx_t high = 1;
    while ((high * (high + 1)) / 2 <= n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;   /* column index within row r */

    /* Allocate the first row */
    seq_t *prev = alloc_seq_array(1);
    prev[0] = (seq_t)1;

    /* Generate rows 1..r, keeping only the previous row */
    for (idx_t row = 1; row <= r; ++row) {
        idx_t cur_len = row + 1;
        seq_t *curr = alloc_seq_array(cur_len);

        /* First element (k = 0) */
        if (row >= 2)      /* prev[1] exists for row >= 2 */
            curr[0] = 2 * prev[0] + prev[1];
        else
            curr[0] = 2 * prev[0];

        /* Middle elements (k = 1 .. row-1) */
        for (idx_t j = 1; j < row; ++j) {
            seq_t value = prev[j - 1];
            if (j + 1 <= row - 1)
                value += prev[j + 1];
            curr[j] = value;
        }

        /* Last element (k = row) */
        curr[row] = prev[row - 1];

        /* Prepare for next iteration */
        free_seq_array(prev);
        prev = curr;
    }

    /* Retrieve the requested entry and clean up */
    seq_t result = prev[k];
    free_seq_array(prev);
    return result;
}
