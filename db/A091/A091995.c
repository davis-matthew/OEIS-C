/*
 * A091995  Permutation of the natural numbers.
 * https://oeis.org/A091995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 155, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   diag_prefix_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements A091995: the numbers 1,2,3,… arranged in a
   triangular array (row i has i numbers) and then read along the
   antidiagonals in order of increasing row index within each diagonal.
   The sequence is computed from the position of the n‑th element in this
   antidiagonal ordering. */
#include <stdint.h>
#include "../utils/diag_prefix_sum.h"

/* Return ⌊√n⌋ for n up to 2^64−1.  Uses only integer arithmetic. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = 1ULL << 32;   /* 2^32 is > √(2^64) */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (mid == 0) { high = 1; continue; }
        if (n / mid >= mid)  /* mid <= √n  →  keep mid */
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* only defined for n≥1 */
    idx_t m = floor_sqrt_u64(n);           /* m = ⌊√n⌋,  m² ≤ n < (m+1)² */

    /* Find diagonal sum k that contains position n. */
    idx_t k;
    if (n == m * m)
        k = 2 * m;                          /* even diagonal */
    else if (n <= m * (m + 1))
        k = 2 * m + 1;                      /* odd diagonal   */
    else
        k = 2 * (m + 1);                    /* next even diag */

    /* Offset of n within diagonal k (0‑based). */
    idx_t prevCum = diag_prefix_sum(k - 1);
    idx_t offset  = n - prevCum - 1;

    /* Row index i = ceil(k/2) + offset.  Column j = k − i. */
    idx_t firstRow = (k + 1) >> 1;          /* ⌈k/2⌉ */
    idx_t i = firstRow + offset;           /* 1 ≤ i ≤ k−1 */
    idx_t j = k - i;                       /* 1 ≤ j ≤ i */

    /* Value at (i,j) in the triangle: T(i‑1)+j where T(k)=k(k+1)/2. */
    seq_t r = (seq_t)i;
    seq_t c = (seq_t)j;
    seq_t val = ((r - 1) * r) / 2 + c;
    return val;
}
