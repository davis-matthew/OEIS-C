/*
 * A208058  Triangle by rows relating to the factorials, generated from
 *          A002260.
 * https://oeis.org/A208058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For row k (0‑based) and column j (0‑based), t(k,j) = (j+1) *
 *            ∏_{m=j+1}^{k-1} m for j < k, and t(k,k) = 1. The sequence a(n)
 *            is the enumeration of this triangle row by row with offset 0
 * Range:     u8 n <= 27, u16 n <= 45, u32 n <= 104, u64 n <= 252,
 *            u128 n <= 630, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* Find the row index k such that n lies in that row. */
    idx_t k = (_floor_isqrt_u64(8 * n + 1) - 1) / 2;

    /* Number of elements preceding row k. */
    idx_t prev = k * (k + 1) / 2;

    /* Column index within the row. */
    idx_t j = n - prev;

    if (j == k)   /* last entry in the row */
        return (seq_t)1;

    /* Compute t(k,j) = (j+1) * (j+1) * (j+2) * … * (k-1) */
    seq_t result = (seq_t)(j + 1);
    for (idx_t m = j + 1; m <= k - 1; ++m)
        result *= (seq_t)m;

    return result;
}
