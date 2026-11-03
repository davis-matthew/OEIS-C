/*
 * A105567  Moebius transform of multiplication table, read by antidiagonals.
 * https://oeis.org/A105567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = -μ(k*(row-k)) where row = row_of_index(n) is such that
 *            row*(row+1)/2 ≤ n < (row+1)*(row+2)/2 k = n - row*(row+1)/2
 *            (column index within the row). For k* (row-k) = 0 we take μ(0) =
 *            -1 to obtain a value of +1
 * Range:     i8 n <= 109, i16 n <= 109, i32 n <= 109, i64 n <= 109,
 *            i128 n <= 109, big any n
 * Validated: partial, pass, 110 terms through n = 109 (2026-11-01)
 * Helpers:   mobius_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius_idx.h"

static inline idx_t row_of_index(idx_t n)
{
    /* Binary search for the largest r with r*(r+1)/2 <= n. */
    idx_t low = 0, high = n, mid, tri;
    idx_t r = 0;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        tri = mid * (mid + 1) / 2;
        if (tri <= n) { r = mid; low = mid + 1; }
        else           high = mid - 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t row = row_of_index(n);
    idx_t col = n - row * (row + 1) / 2;
    idx_t prod = col * (row - col);      /* k*(row-k) */

    if (prod == 0)                       /* μ(0) is treated as -1 */
        return (seq_t)1;
    else
        return -(seq_t)mobius_idx(prod);     /* -μ(prod) */
}
