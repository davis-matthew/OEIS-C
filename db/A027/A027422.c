/*
 * A027422  Triangle T00, T10, T11, T20, T21, T22, etc. where Tmn = number of
 *          distinct products ij with m-n<=i,j<=m.
 * https://oeis.org/A027422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 431, u16 n <= 1890, u32 n <= 1890, u64 n <= 1890,
 *            u128 n <= 1890, big any n
 * Helpers:   distinct_product_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the number of distinct products i·j with m‑k ≤ i,j ≤ m
   where (m,k) are obtained from the flat index n via the triangle
   T_{m,k}.  The mapping is defined by
      n = (m+1)(m+2)/2 + k   with 0 ≤ k ≤ m. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/distinct_product_count.h"

seq_t a(idx_t n)
{
    /* Find row m such that (m+1)(m+2)/2 > n. */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n) ++m;

    /* compute k within the row */
    idx_t prev_tri = m * (m + 1) / 2;  /* number of terms before row m */
    idx_t k = n - prev_tri;

    return distinct_product_count(m, k);
}
